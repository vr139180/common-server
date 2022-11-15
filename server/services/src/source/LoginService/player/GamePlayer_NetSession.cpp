// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "player/GamePlayer.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>
#include <cmsLib/prolib/ProtocolFactory.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/global_const.h>
#include <gameLib/LogExt.h>

#include "player/PlayerChannel.h"
#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"


GamePlayer::GamePlayer():slot_(-1)
, cur_state_(PlayerState::PlayerState_Free)
, role_iid_(0)
{
	session_.reset(new NetSession(0, 0, this));

	this->init_protocolhead();
}

GamePlayer::~GamePlayer()
{
}

void GamePlayer::init_protocolhead()
{
	s_head_.router_balance_ = true;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = PRO::ERK_SERVICE_GATE;
	s_head_.to_type_ = -1;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
	s_head_.token_gidrid_ = 0;
	s_head_.token_slottoken_ = 0;
}

NetProtocol* GamePlayer::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	NetProtocol* pro = new NetProtocol( get_protocolhead(), ntf);

	return pro;
}

void GamePlayer::reset()
{
	cur_state_ = PlayerState::PlayerState_Free;
	session_->force_reset();
}

void GamePlayer::force_close()
{
	session_->force_reset();
}

void GamePlayer::update(u64 tnow)
{
	if (cur_state_ == PlayerState_Loginning)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (start_timestamp_ + 1000 * 15 < tnow)
		{
			cur_state_ = PlayerState_Closing;

			PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot_);

			SystemCommand<GamePlayer>* cmd =
				new SystemCommand<GamePlayer>(boost::bind(&PlayerChannel::on_connection_timeout, pchannel, this));
			pchannel->regist_syscmd(cmd);
		}
	}
	else if (is_auth())
	{
		heart_beat();
	}
}

NetProtocol* GamePlayer::unpack_protocol(S_UINT_8 *pbuf, S_UINT_32 prolen)
{
	NetProtocol* pro = new NetProtocol();
	std::unique_ptr<NetProtocol> ptr(pro);

	S_UINT_8 *pdata = pbuf;

	CProtocolHead chead;
	if (!chead.decode_head(pdata, prolen))
		return 0;

	//是否需要对协议解码
	chead.unpack_protocol_ = this->is_need_unpack_protocol(chead.get_msgid());

	pdata = pbuf + (S_UINT_32)chead.get_headlen();

	if (chead.unpack_protocol_)
	{
		ProtocolFactory* fact = ProtocolFactory::instance();
		try {
			BasicProtocol* bp = fact->iid_to_proto(chead.get_msgid(), pdata, chead.get_msglen());
			if (bp == 0)
				return 0;

			pro->msg_ = bp;
		}
		catch (...) {
			return 0;
		}
	}
	else
	{
		//不解析协议，保存单独的内存块
		pro->data_len_ = chead.get_msglen();
		pro->msg_data_ = new S_UINT_8[pro->data_len_];
		memcpy(pro->msg_data_, pdata, pro->data_len_);
	}

	SProtocolHead& head = pro->head_;
	head.unpack_protocol_ = chead.unpack_protocol_;
	head.set_msgid(chead.get_msgid());
	head.router_balance_ = false;

	return ptr.release();
}

bool GamePlayer::pack_protocol(S_UINT_8 *pbuf, S_UINT_32 maxlen, NetProtocol* pro, S_UINT_32& prolen)
{
	prolen = 0;

	SProtocolHead& shead = pro->head_;

	CProtocolHead head = c_head_;
	head.unpack_protocol_ = shead.unpack_protocol_;

	S_UINT_8 *pdata = pbuf;

	if (head.unpack_protocol_)
	{
		if (pro->msg_ == 0)
			return false;

		ProtocolFactory* fact = ProtocolFactory::instance();
		S_UINT_16 proiid = fact->proto_to_iid(pro->msg_);
		if (proiid == 0)
			return false;
		//设置协议id
		head.set_msgid(proiid);
	}

	if (!head.encode_head(pdata, maxlen))
		return false;

	if (head.unpack_protocol_)
	{
		S_UINT_32 len2 = (S_UINT_32)pro->msg_->ByteSizeLong();

		if (head.get_headlen() + len2 > maxlen)
			return false;

		try {
			pdata = pbuf + head.get_headlen();
			if (!pro->msg_->SerializeToArray(pdata, len2))
				return false;
		}
		catch (...) {
			return false;
		}

		head.encode_totlelen(pbuf, maxlen, len2);
	}
	else
	{
		if (pro->msg_data_ == 0 && pro->data_len_ > 0)
			return false;

		S_UINT_32 len2 = (S_UINT_32)pro->data_len_;
		if (head.get_headlen() + len2 > maxlen)
			return false;

		pdata = pbuf + head.get_headlen();
		memcpy(pdata, pro->msg_data_, len2);

		head.encode_totlelen(pbuf, maxlen, len2);
	}

	//写长度
	prolen = head.get_totlelen();

	return true;
}