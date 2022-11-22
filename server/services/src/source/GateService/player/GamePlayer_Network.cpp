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

#include <cmsLib/base/OSSystem.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

void GamePlayer::on_connect_lost_netthread()
{
	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot_);

	SystemCommand<GamePlayer>* cmd =
		new SystemCommand<GamePlayer>(boost::bind(&PlayerChannel::on_disconnected_with_player, pchannel, this));
	pchannel->regist_syscmd(cmd);
}

bool GamePlayer::is_need_unpack_protocol(S_UINT_16 msgid)
{
	return true;
}

void GamePlayer::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::USER_LOGIN_REQ || msgid == PRO::USER_RELOGIN_REQ)
	{
		if (cur_state_ != PlayerState_Free)
			return;

		this->cur_state_ = PlayerState::PlayerState_Loginning;
		svrApp.route_to_datarouter(PRO::ERK_SERVICE_STATE, p_msg.release());
	}
	else if (msgid == PRO::USER_ROLECREATE_REQ || msgid == PRO::USER_ROLESELECT_REQ 
		|| msgid == PRO::USER_ROLELIST_REQ)
	{
		if( !is_in_rolerange())
			return;

		svrApp.route_to_datarouter( PRO::ERK_SERVICE_HOME, p_msg.release());
	}
	else if (msgid == PRO::USER_ACTIVE_NTF)
	{
		if (!is_login())
			return;

		svrApp.route_to_datarouter(PRO::ERK_SERVICE_STATE, p_msg.release());
	}
	else
	{
		if( !is_roleready())
			return;

		if(msgid > PRO::CHAT_PROTYPE::CHAT_MSG_BEGIN && msgid < PRO::CHAT_PROTYPE::CHAT_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_CHAT, p_msg.release());
			return;
		}
		else if (msgid > PRO::MAIL_PROTYPE::MAIL_MSG_BEGIN && msgid < PRO::MAIL_PROTYPE::MAIL_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_MAIL, p_msg.release());
			return;
		}
		else if (msgid > PRO::FRIEND_PROTYPE::FRIEND_MSG_BEGIN && msgid < PRO::FRIEND_PROTYPE::FRIEND_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_FRIEND, p_msg.release());
			return;
		}
		else if (msgid > PRO::TASK_PROTYPE::TASK_MSG_BEGIN && msgid < PRO::TASK_PROTYPE::TASK_MSG_END)
		{
			svrApp.route_to_datarouter(PRO::ERK_SERVICE_HOME, p_msg.release());
			return;
		}
		else //default, route to datarouter
		{
			svrApp.route_to_datarouter(PRO::ERK_SERVICE_HOME, p_msg.release());
		}
	}
}

void GamePlayer::registinfo_tolog(bool bregist)
{
	if (bregist)
	{
		logInfo(out_runtime, "Player[%d] auth to me(GateService)", get_iid());
	}
	else
		logInfo(out_runtime, "Player[%d] disconnect from me(GateService)", get_iid());
}