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

	REGISTERMSG(GMS_PROTYPE::GMS_ENTERGAME_ACK, &PlayerChannel::on_pc_entergame_ntf, this);
	REGISTERMSG(GMS_PROTYPE::GMS_SAVEUSERLOC_NTF, &PlayerChannel::on_pc_saveuserloc_ntf, this);

	REGISTERMSG(CHAT_PROTYPE::CHAT_GLOBALMSG_NTF, &PlayerChannel::on_pc_broadcast_chat_globalmsg, this);
}

void PlayerChannel::on_disconnected_with_player(GamePlayer* plink)
{
	if (plink->trigger_gatelost())
	{
		User_GateLost_ntf* ntf = new User_GateLost_ntf();
		plink->send_to_state(ntf);
	}

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

	//设置状态
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

	//设置状态
	if (ack->result() == 0)
	{
		//auth check
		parent_->auth_wait_slot(puser->get_userslot());

		//设置相关状态
		puser->auth(ack->user_iid(), ack->logintoken());
		if (ack->role_iid() > 0)
		{
			puser->set_gameid(ack->gameid());

			const Location3D& xpos = ack->role_pos();
			GLoc3D pos;
			pos.set_x(xpos.x());
			pos.set_y(xpos.y());
			pos.set_z(xpos.z());

			puser->role_selected_done(ack->role_iid(), pos);
		}
		
		puser->registinfo_tolog(true);
	}

	puser->send_netprotocol(ack);
}

void PlayerChannel::on_pc_userlogout_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	//主动断开，不触发gatelost
	puser->set_gatelost_untrigger();

	puser->force_close();
}

void PlayerChannel::on_pc_userlogout_force_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	autorelease = false;
	send_msg_to_player(pro);

	//state检测强制断开，不触发gatelost
	puser->set_gatelost_untrigger();

	force_pc_close_player(puser);
}

void PlayerChannel::on_pc_roleselect_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	User_RoleSelect_ack *ack = dynamic_cast<User_RoleSelect_ack*>(pro->msg_);
	if (ack->result() == 0)
	{
		const Location3D& xpos = ack->loc();
		GLoc3D pos;
		pos.set_x(xpos.x());
		pos.set_y(xpos.y());
		pos.set_z(xpos.z());

		puser->role_selected_done(ack->role_iid(), pos);

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

	Game_EnterGame_ack *ntf = dynamic_cast<Game_EnterGame_ack*>(pro->msg_);
	if (ntf->result() == 0)
	{
		puser->set_gameid(ntf->game_iid());
		logDebug(out_runtime, "user:%lld enter game succc, gameid:%lld", puser->get_iid(), puser->get_gameid());

		Location3D* xpos = ntf->mutable_pos();
		puser->update_location_from(xpos);

		puser->force_user_active();
	}

	autorelease = false;
	puser->send_netprotocol( pro);
}

void PlayerChannel::on_pc_saveuserloc_ntf(NetProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	{
		Game_SaveUserLoc_ntf* ntf = dynamic_cast<Game_SaveUserLoc_ntf*>(pro->msg_);
		PRO::Location3D* pos = ntf->mutable_loc();
		puser->update_location_from(pos);
	}

	//to state
	puser->force_user_active();

	//to homeservice
	{
		User_SaveRoleLoc_ntf *xntf = new User_SaveRoleLoc_ntf();
		Location3D* xpos = xntf->mutable_role_pos();
		puser->copy_location_to(puser->get_game_loc(), xpos);

		puser->send_to_home(xntf);
	}
}