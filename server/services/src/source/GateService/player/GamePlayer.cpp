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
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <gameLib/LogExt.h>

#include "GateServiceApp.h"

void GamePlayer::init(int s)
{
	this->slot_ = s;
	this->cur_state_ = PlayerState::PlayerState_Free;
	this->role_iid_ = 0;
	this->user_iid_ = 0;
	this->game_loc_ = GLoc3D::zero_point();
	this->game_iid_ = 0;
}

void GamePlayer::reuse()
{
	this->reset();
}

void GamePlayer::force_linkclose()
{
	force_close();
}

bool GamePlayer::trigger_gatelost()
{
	if (!is_auth())
		return false;

	if (b_trige_gatelost_)
	{
		b_trige_gatelost_ = false;
		return true;
	}

	return false;
}

void GamePlayer::pre_start(S_INT_64 gateid)
{
	//slot+token只在pre_start时设置一次
	s_head_.set_token_slottoken(slot_, ShareUtil::get_token());
	s_head_.set_token_gateid(gateid);

	//-----------------------------------------------
	cur_state_ = PlayerState::PlayerState_Free;
	this->user_iid_ = 0;
	this->role_iid_ = 0;
	this->game_loc_ = GLoc3D::zero_point();
	this->game_iid_ = 0;
	this->start_timestamp_ = OSSystem::mOS->GetTimestamp();
	b_trige_gatelost_ = true;
}

void GamePlayer::auth( S_INT_64 uid, S_INT_64 token)
{
	cur_state_ = PlayerState_Logon;
	user_iid_ = uid;
	s_head_.set_token_userid( uid);
	s_head_.set_token_token(token);
}

void GamePlayer::role_selected_done(S_INT_64 rid, const GLoc3D& pos)
{
	cur_state_ = PlayerState::PlayerState_RoleReady;
	role_iid_ = rid;
	game_loc_ = pos;
	s_head_.set_role_iid( role_iid_);

	PRO::Game_EnterGame_req* req = new PRO::Game_EnterGame_req();
	ProtoUtil::set_location_to_msg(req, game_loc_);
	req->set_game_iid(game_iid_);

	send_to_game(req);
}

void GamePlayer::set_gameid(S_INT_64 gid)
{
	game_iid_ = gid;
	s_head_.set_gameid(game_iid_);

	logDebug(out_runtime, "-----set gameid:%lld", gid);
}

void GamePlayer::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	session_->send_protocol(pro);
}

void GamePlayer::send_to_state(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	svrApp.route_to_datarouter(PRO::ERK_SERVICE_STATE, pro);
}

void GamePlayer::send_to_home(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	svrApp.route_to_datarouter(PRO::ERK_SERVICE_HOME, pro);
}

void GamePlayer::send_to_game(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	svrApp.route_to_fightrouter(PRO::ERK_SERVICE_GAME, pro);
}

void GamePlayer::copy_location_to(PRO::Location3D* pos, GLoc3D& loc)
{
	loc.set_x(pos->x());
	loc.set_y(pos->y());
	loc.set_z(pos->z());
}

void GamePlayer::copy_location_to(const GLoc3D& loc, PRO::Location3D* pos)
{
	pos->set_x(loc.x());
	pos->set_y(loc.y());
	pos->set_z(loc.z());
}

void GamePlayer::update_location_from(PRO::Location3D* pos)
{
	copy_location_to(pos, game_loc_);

	logDebug(out_runtime, "user:%lld save new loc:%s", user_iid_, game_loc_.to_string().c_str());
}