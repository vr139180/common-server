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

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>

#include "dbs/cmd/LoadUserInfoCmd.h"
#include "dbs/cmd/CreateUserRoleCmd.h"

#include "dbs/DBSCtrl.h"

#include "HomeServiceApp.h"

void LobbyUser::on_ls_rolelist_req()
{
	if (cur_state_ < UserState::UserState_RolesReady)
		return;

	PRO::User_RoleList_ack *ack = new PRO::User_RoleList_ack();

	ack->set_allocated_roles(roles_data_.clone_data<PRO::DBUserRoles>());

	svrApp.send_protocol_to_gate(ack);
}

void LobbyUser::on_ls_rolecreate_req(const char* nickname)
{
	if (is_user_ready())
	{
		PRO::User_RoleCreate_ack* ack = new PRO::User_RoleCreate_ack();

		ack->set_result(1); //不允许创建角色
		svrApp.send_protocol_to_gate(ack);

		return;
	}

	CreateUserRoleCmd* pcmd = new CreateUserRoleCmd( s_head_, owner_);
	pcmd->ls_create_new_role(nickname);

	dbsStore->post_db_cmd(pcmd);
}

void LobbyUser::role_selected_done()
{
	cur_state_ = UserState::UserState_Ready;

	PRO::User_RoleSelect_ack* ack = new PRO::User_RoleSelect_ack();

	ack->set_role_iid(role_iid_);
	ack->set_result(0);
	svrApp.send_protocol_to_gate(ack);

	//更新giduid，设置uid为role_iid
	S_INT_64 gateid = 0;

	notify_roledetail_to_user();
}

void LobbyUser::on_ls_roleselect_req(S_INT_64 roleid)
{
	int r = 0;
	if (!roles_data_.is_role_exist(roleid))
		r = 1; //role不存在
	if (r == 0 && cur_state_ != UserState::UserState_RolesReady)
		r = 2; //role已经选择

	if (r != 0)
	{
		PRO::User_RoleSelect_ack* ack = new PRO::User_RoleSelect_ack();

		ack->set_role_iid(roleid);
		ack->set_result(r);
		svrApp.send_protocol_to_gate(ack);

		return;
	}
	
	//设置当前role
	this->set_role_iid(roleid);
	if (this->sync_all())
	{
		role_selected_done();
	}
	else
	{
		cur_state_ = UserState::UserState_RoleDetailLoading;

		BaseDBCmd* pcmd = new LoadUserInfoCmd( s_head_, owner_);
		dbsStore->post_db_cmd(pcmd);
	}
}

void LobbyUser::notify_roledetail_to_user()
{
	PRO::User_RoleDetailA_ntf * na = new PRO::User_RoleDetailA_ntf();

	na->set_allocated_home(home_data_.clone_data<PRO::DBUserHome>());
	na->set_allocated_homeitems(building_data_.clone_data<PRO::DBUserHomeStructure>());

	svrApp.send_protocol_to_gate( na);

	PRO::User_RoleDetailB_ntf* nb = new PRO::User_RoleDetailB_ntf();

	nb->set_allocated_pets(pet_data_.clone_data<PRO::DBUserPets>());

	svrApp.send_protocol_to_gate(nb);

	PRO::User_RoleDetailEnd_ntf *en = new PRO::User_RoleDetailEnd_ntf();

	svrApp.send_protocol_to_gate(en);
}
