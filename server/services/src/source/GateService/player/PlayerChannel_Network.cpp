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
	REGISTERMSG(USER_PROTYPE::USER_LOGIN_ACK, &PlayerChannel::on_pc_userlogin_ack, this);
	REGISTERMSG(USER_PROTYPE::USER_RELOGIN_ACK, &PlayerChannel::on_pc_userrelogin_ack, this);

	REGISTERMSG(USER_PROTYPE::USER_LOGOUT_NTF, &PlayerChannel::on_pc_userlogout_ntf, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLESELECT_ACK, &PlayerChannel::on_pc_roleselect_ack, this);

	REGISTERMSG(CHAT_PROTYPE::CHAT_GLOBALMSG_NTF, &PlayerChannel::on_pc_broadcast_chat_globalmsg, this);
}

void PlayerChannel::on_disconnected_with_player(GamePlayer* plink)
{
	User_GateLost_ntf* ntf = new User_GateLost_ntf();
	plink->send_to_state(ntf);

	force_pc_close_player(plink);
}

void PlayerChannel::on_pc_userlogin_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = get_player_bytoken(pro);
	if (puser == 0)
		return;

	User_Login_ack* ack = dynamic_cast<User_Login_ack*>(pro->msg_);

	logDebug(out_runtime, "user:%lld login:%d", ack->user_iid(), ack->result());

	autorelease = false;

	//ÉèÖÃ×´Ì¬
	if (ack->result() == 0)
	{
		//auth check
		parent_->auth_wait_slot(puser->get_userslot());

		puser->auth(ack->user_iid(), ack->logintoken());
		puser->registinfo_tolog(true);
	}

	puser->send_netprotocol(ack);
}

void PlayerChannel::on_pc_userrelogin_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = get_player_bytoken(pro);
	if (puser == 0)
		return;

	User_ReLogin_ack* ack = dynamic_cast<User_ReLogin_ack*>(pro->msg_);

	logDebug(out_runtime, "user:%lld relogin:%d", ack->user_iid(), ack->result());

	autorelease = false;

	//ÉèÖÃ×´Ì¬
	if (ack->result() == 0)
	{
		//auth check
		parent_->auth_wait_slot(puser->get_userslot());

		//ÉèÖÃÏà¹Ø×´Ì¬
		puser->auth(ack->user_iid(), ack->logintoken());
		if (ack->role_iid() > 0)
			puser->role_selected_done(ack->role_iid());
		puser->set_gameid(ack->gameid());
		
		puser->registinfo_tolog(true);
	}

	puser->send_netprotocol(ack);
}

void PlayerChannel::on_pc_userlogout_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	User_Logout_ntf* ntf = dynamic_cast<User_Logout_ntf*>(pro->msg_);
	ntf->set_user_iid(puser->get_iid());
	ntf->set_role_iid(puser->get_roleiid());
	ntf->set_gameid(puser->get_gameid());

	autorelease = false;
	svrApp.route_to_datarouter(PRO::ERK_SERVICE_STATE, pro);

	force_pc_close_player(puser);
}

void PlayerChannel::on_pc_roleselect_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	User_RoleSelect_ack *ack = dynamic_cast<User_RoleSelect_ack*>(pro->msg_);
	if (ack->result() == 0)
	{
		puser->role_selected_done(ack->role_iid());

		puser->force_user_active();
	}

	autorelease = false;
	puser->send_netprotocol(ack);
}

void PlayerChannel::on_pc_broadcast_chat_globalmsg(NetProtocol* pro, bool& autorelease)
{
	Chat_GlobalMsg_ntf *ntf = dynamic_cast<Chat_GlobalMsg_ntf*>(pro->msg_);
	/*
	for (ONLINE_PLAYER_MAP::iterator iter = online_users_.begin(); iter != online_users_.end(); ++iter)
	{
		GamePlayer *p = iter->second;
		
		Chat_GlobalMsg_ntf* msg = new Chat_GlobalMsg_ntf();
		msg->CopyFrom(*ntf);

		p->send_netprotocol( msg);
	}
	*/
}

void PlayerChannel::on_pc_entergame_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	Game_EnterGame_ntf *ntf = dynamic_cast<Game_EnterGame_ntf*>(pro->msg_);
	puser->set_gameid(ntf->game_iid());

	puser->force_user_active();

	autorelease = false;
	puser->send_netprotocol( pro);
}
