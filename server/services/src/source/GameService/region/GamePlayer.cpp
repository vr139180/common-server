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

#include "region/GamePlayer.h"

#include <gameLib/protobuf/Proto_all.h>
#include "region/RegionCellNode.h"

#include "GameServiceApp.h"

GamePlayer::GamePlayer():
	b_master_node_(true),
	player_state_( GamePlayerState::GamePlayerS_Free),
	user_iid_(0),
	role_iid_(0),
	owner_cellnode_(0)
{
}

void GamePlayer::reset()
{
	b_master_node_ = true;
	player_state_ = GamePlayerState::GamePlayerS_Free;
	s_head_ = SProtocolHead();
	
	user_iid_ = 0;
	role_iid_ = 0;
	nickname_ = "";
	location_ = GLoc3D::zero_point();
}

void GamePlayer::copy_user_info(PRO::GameUserInfo* pui)
{
	pui->set_user_iid(user_iid_);
	pui->set_role_iid(role_iid_);
	pui->set_nickname(nickname_.c_str());
	PRO::Location3D* pos = pui->mutable_pos();
	GLoc3D::copy_to(location_, pos);
}

void GamePlayer::send_to_gate(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(s_head_, msg);
	svrApp.send_protocol_to_gate(pro);
}

void GamePlayer::send_to_home(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(s_head_, msg);
	svrApp.send_protocol_to_home(pro);
}

bool GamePlayer::enter_game(const SProtocolHead& head, S_INT_64 gameid, const GLoc3D& loc)
{
	if (player_state_ == GamePlayerState::GamePlayerS_Free)
	{
		//新进入的玩家
		//请求最新的角色信息
		player_state_ = GamePlayerState::GamePlayerS_EnterWaitDetail;

		s_head_.sync_token(head);
		s_head_.set_gameid(gameid);

		user_iid_ = head.get_token_useriid();
		role_iid_ = head.get_role_iid();
		nickname_ = "";
		location_ = loc;

		//请求角色详情
		PRO::User_MySimpleInfo_req* req = new PRO::User_MySimpleInfo_req();
		this->send_to_home(req);

		logDebug(out_runtime, "set free userinfo, uid:%lld rid:%lld", user_iid_, role_iid_);
	}
	else
	{
		//可能是断线重连等导致的
		s_head_.sync_token(head);
		s_head_.set_gameid(gameid);

		role_iid_ = head.get_role_iid();
		location_ = loc;

		logDebug(out_runtime, "set exist userinfo, uid:%lld rid:%lld", user_iid_, role_iid_);
	}

	return true;
}

bool GamePlayer::on_myinfo_get(BasicProtocol* msg)
{
	if (player_state_ == GamePlayerState::GamePlayerS_Free)
		return false;

	PRO::User_MySimpleInfo_ack* ack = dynamic_cast<PRO::User_MySimpleInfo_ack*>(msg);

	this->nickname_ = ack->nickname().c_str();
	player_state_ = GamePlayerState::GamePlayerS_Ready;

	return true;
}

void GamePlayer::net_save_gameloc()
{
	PRO::Game_SaveUserLoc_ntf* ntf = new PRO::Game_SaveUserLoc_ntf();
	PRO::Location3D* pos = ntf->mutable_loc();
	GLoc3D::copy_to(location_, pos);

	send_to_gate(ntf);

	logDebug(out_runtime, "user:%lld save loc:%s to gate, state, home", user_iid_, location_.to_string().c_str());
}