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

GamePlayer::GamePlayer():b_master_node_(true),
player_state_( GamePlayerState::GamePlayerS_Free)
{
}

void GamePlayer::copy_location(const GLoc3D& loc, PRO::Location3D* pos)
{
	pos->set_x(loc.x());
	pos->set_y(loc.y());
	pos->set_z(loc.z());
}

void GamePlayer::reset()
{
	b_master_node_ = true;
	player_state_ = GamePlayerState::GamePlayerS_Free;
	s_head_ = SProtocolHead();
}

void GamePlayer::sync_head(const SProtocolHead& head, S_INT_64 gameid, const GLoc3D& loc)
{
	s_head_.sync_token(head);
	s_head_.set_gameid(gameid);

	user_simple_info_.set_user_iid(head.get_token_useriid());
	user_simple_info_.set_role_iid(head.get_role_iid());
	
	user_simple_info_.set_nickname("");
	PRO::Location3D* pos = user_simple_info_.mutable_pos();
	copy_location(loc, pos);
}
