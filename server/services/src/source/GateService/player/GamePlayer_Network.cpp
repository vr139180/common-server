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
#include <gameLib/protobuf/ProtoUtil.h>

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

void GamePlayer::force_user_active()
{
	PRO::User_Active_ntf* ntf = new PRO::User_Active_ntf();
	
	PRO::Location3D* pos = ntf->mutable_role_pos();
	pos->set_x(game_loc_.x());
	pos->set_y(game_loc_.y());
	pos->set_z(game_loc_.z());

	this->send_to_state(ntf);
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

		PRO::User_Active_ntf* ntf = dynamic_cast<PRO::User_Active_ntf*>(pro->msg_);
		
		PRO::Location3D* pos = ntf->mutable_role_pos();
		pos->set_x(game_loc_.x());
		pos->set_y(game_loc_.y());
		pos->set_z(game_loc_.z());

		svrApp.route_to_datarouter(PRO::ERK_SERVICE_STATE, p_msg.release());

		//如果有gameservice服务，需要定时去激活
		if (game_iid_ > 0)
		{
			PRO::Game_UserAlive_ntf* ntf = new PRO::Game_UserAlive_ntf();
			ProtoUtil::set_location_to_msg(ntf, game_loc_);

			send_to_game(ntf);
		}
	}
	else if (msgid == PRO::USER_LOGOUT_NTF)
	{
		if (!is_login())
			return;

		svrApp.dispatch_gate_process(p_msg.release());
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
		else if (msgid > PRO::GMS_PROTYPE::GMS_MSG_BEGIN && msgid < PRO::GMS_PROTYPE::GMS_MSG_END)
		{
			if( msgid != PRO::GMS_PROTYPE::GMS_USERSTATE_SYN)
				ProtoUtil::set_location_to_msg(pro->msg_, this->game_loc_);

			if (msgid == PRO::GMS_PROTYPE::GMS_ENTERGAME_REQ)
			{
				PRO::Game_EnterGame_req* req = dynamic_cast<PRO::Game_EnterGame_req*>(pro->msg_);
				req->set_game_iid(game_iid_);
			}

			svrApp.route_to_fightrouter(PRO::ERK_SERVICE_GAME, p_msg.release());
			return;
		}
		else if( msgid > PRO::USER_PROTYPE::USER_MSG_BEGIN && msgid < PRO::USER_PROTYPE::USER_MSG_END)
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

void GamePlayer::notify_state_logout()
{
	PRO::User_Logout_ntf* ntf = new PRO::User_Logout_ntf();
	ntf->set_user_iid(this->user_iid_);
	ntf->set_role_iid(this->role_iid_);
	ntf->set_token(s_head_.get_token_token());
	ntf->set_gameid(this->game_iid_);

	send_to_state(ntf);
}