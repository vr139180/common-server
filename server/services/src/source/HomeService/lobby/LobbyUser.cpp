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

void LobbyUser::rest_user()
{
	this->cur_state_ = UserState::UserState_Free;
	
	this->roles_data_.reset_data();
	this->reset_usercache();
}

void LobbyUser::send_to_gate(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(s_head_, msg);
	SProtocolHead& head = pro->write_head();
	head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_HOME;
	head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

	svrApp.send_netprotocol(pro);
}

void LobbyUser::send_to_game(BasicProtocol* msg, S_INT_64 gameid)
{
	NetProtocol* pro = new NetProtocol(s_head_, msg);
	SProtocolHead& head = pro->write_head();

	head.set_gameid(gameid);
	head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_HOME;
	head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GAME;

	svrApp.send_netprotocol(pro);
}

void LobbyUser::sync_head(const SProtocolHead& head)
{
	// SH:LJFU:TODO
	// 在home发生切换时，如何从redis同步最新数据
	// 
	if (cur_state_ == UserState::UserState_Free)
	{
		s_head_ = head;

		redis_sync_rolelist();
		cur_state_ = UserState::UserState_RolesReady;
		
		S_INT_64 roleid = s_head_.get_role_iid();
		if (roleid <= 0)
			return;

		//如果有role加载它
		reset_usercache();
		set_role_iid(roleid);

		if (sync_all())
		{
			cur_state_ = UserState::UserState_Ready;
		}
		else
		{
			cur_state_ = UserState::UserState_RoleDetailLoading;

			//数据库中加载
			BaseDBCmd* pcmd = new LoadUserInfoCmd(s_head_, true, owner_);
			dbsStore->post_db_cmd(pcmd);
		}
	}
	else
	{
		//only sync token part
		s_head_.sync_token(head);
	}
}
