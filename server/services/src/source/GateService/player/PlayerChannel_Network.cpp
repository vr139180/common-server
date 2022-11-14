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

#include "player/PlayerChannel.h"

#include <gameLib/LogExt.h>
#include <gameLib/eureka/EurekaClusterClient.h>

#include <gameLib/protobuf/Proto_all.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void PlayerChannel::InitNetMessage()
{
	REGISTERMSG(USER_PROTYPE::USER_LOGOUT_NTF, &PlayerChannel::on_pc_userlogout_ntf, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLESELECT_ACK, &PlayerChannel::on_pc_roleselect_ack, this);

	REGISTERMSG(CHAT_PROTYPE::CHAT_GLOBALMSG_NTF, &PlayerChannel::on_pc_broadcast_chat_globalmsg, this);
}

void PlayerChannel::on_disconnected_with_player(GamePlayer* plink)
{
	force_pc_close_player(plink);
}

void PlayerChannel::on_cth_userproxylogin_req(NetProtocol* message, bool& autorelease, void* p)
{
	GamePlayer *psession = (GamePlayer*)p;

	User_ProxyLogin_ack* ack = new User_ProxyLogin_ack();
	ack->set_result(1);

	User_ProxyLogin_req* req = dynamic_cast<User_ProxyLogin_req*>(message->msg_);

	GamePlayer* pLink = get_player_byslot(psession->get_userslot());
	//TODO:disconnected
	if (pLink == 0 || pLink != psession)
		return;

	//token check
	if (pLink->is_samesession( message->head_))
	{
		pLink->send_netprotocol(ack);
		return;
	}

	//auth check
	if (parent_->auth_wait_slot(pLink->get_userslot()) == false)
	{
		pLink->send_netprotocol(ack);
		return;
	}

	online_users_[pLink->get_iid()] = pLink;
	online_user_nums_ = online_users_.size();

	pLink->registinfo_tolog(true);

	ack->set_result(0);
	pLink->send_netprotocol(ack);

	logDebug(out_runtime, "user:%lld login success", req->user_iid());
}

void PlayerChannel::on_pc_userlogout_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	GHS_UserLogout_ntf* ntf = new GHS_UserLogout_ntf();

	//svrApp.send_to_homeservice(ntf);

	force_pc_close_player(puser);
}

void PlayerChannel::on_pc_roleselect_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	User_RoleSelect_ack *ack = dynamic_cast<User_RoleSelect_ack*>(pro->msg_);
	if (ack->result() == 0)
	{
		puser->role_selected_done(ack->role_iid(), EurekaClusterClient::instance().get_myiid());

		//ask role detail
		GHS_RoleDetail_ask *ask = new GHS_RoleDetail_ask();

		//svrApp.send_to_homeservice(ask);
	}

	autorelease = false;
	//puser->send_protocol(pro);
}

void PlayerChannel::on_pc_broadcast_chat_globalmsg(NetProtocol* pro, bool& autorelease)
{
	Chat_GlobalMsg_ntf *ntf = dynamic_cast<Chat_GlobalMsg_ntf*>(pro->msg_);
	for (ONLINE_PLAYER_MAP::iterator iter = online_users_.begin(); iter != online_users_.end(); ++iter)
	{
		GamePlayer *p = iter->second;
		
		Chat_GlobalMsg_ntf* msg = new Chat_GlobalMsg_ntf();
		msg->CopyFrom(*ntf);

		p->send_netprotocol( msg);
	}
}
