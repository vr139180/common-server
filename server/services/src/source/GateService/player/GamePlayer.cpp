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
#include <gameLib/LogExt.h>

#include "GateServiceApp.h"

void GamePlayer::init(int s)
{
	this->slot_ = s;
	this->cur_state_ = PlayerState::PlayerState_Free;
	this->role_iid_ = 0;
	this->user_iid_ = 0;
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

void GamePlayer::role_selected_done(S_INT_64 rid)
{
	cur_state_ = PlayerState::PlayerState_RoleReady;
	role_iid_ = rid;
	s_head_.set_role_iid( role_iid_);
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
