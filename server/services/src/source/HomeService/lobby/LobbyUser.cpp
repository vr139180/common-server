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

#include "lobby/LobbyUser.h"

#include <gameLib/protobuf/Proto_all.h>

#include "dbs/DBSCtrl.h"

#include "dbs/cmd/CreateUserRoleCmd.h"
#include "dbs/cmd/LoadUserRolesCmd.h"
#include "dbs/cmd/LoadUserInfoCmd.h"

#include "lobby/LobbyService.h"

#include "HomeServiceApp.h"

LobbyUser::LobbyUser() :owner_(0), cur_state_(UserState::UserState_Free)
{
}

LobbyUser::~LobbyUser()
{
}

void LobbyUser::set_context(LobbyService* p)
{
	this->owner_ = p;
	task_resolver_.init_env(this->owner_, this, this);
}

void LobbyUser::init_user(S_INT_64 giduid, S_INT_64 slottoken)
{
	this->rest_user();

	S_INT_64 uid = 0;
	this->set_user_iid(uid);

	this->cur_state_ = UserState::UserState_Free;
	
	if (this->sync_rolelist() == false)
	{
		this->cur_state_ = UserState::UserState_RolesLoading;

		//load from database
		BaseDBCmd* pcmd = new LoadUserRolesCmd(s_head_, owner_);
		dbsStore->post_db_cmd(pcmd);
	}
	else
	{
		this->cur_state_ = UserState::UserState_RolesReady;

		//nootify user
		PRO::User_RoleList_ack *ack = new PRO::User_RoleList_ack();

		ack->set_allocated_roles( roles_data_.clone_data<PRO::DBUserRoles>());

		svrApp.send_protocol_to_gate(ack);
	}
}

void LobbyUser::rest_user()
{
	this->cur_state_ = UserState::UserState_Free;
	
	this->roles_data_.reset_data();
	this->reset_usercache();
}
