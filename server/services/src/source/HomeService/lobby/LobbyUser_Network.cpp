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
#include <gameLib/LogExt.h>

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

	send_to_gate(ack);
}

void LobbyUser::on_ls_rolecreate_req(const char* nickname)
{
	if (cur_state_ < UserState::UserState_RolesReady)
		return;

	CreateUserRoleCmd* pcmd = new CreateUserRoleCmd( s_head_, owner_);
	pcmd->ls_create_new_role(nickname);

	dbsStore->post_db_cmd(pcmd);
}

void LobbyUser::on_ls_roleselect_req(S_INT_64 roleid)
{
	S_INT_32 r = 0;
	if (!roles_data_.is_role_exist(roleid))
		r = 1; //role不存在

	if (r != 0)
	{
		PRO::User_RoleSelect_ack* ack = new PRO::User_RoleSelect_ack();

		ack->set_role_iid(roleid);
		ack->set_result(r);
		send_to_gate(ack);

		return;
	}

	//role发生了切换
	if (get_role_iid() != roleid)
	{
		//清除原有数据
		reset_usercache();
		cur_state_ = UserState::UserState_RolesReady;

		logDebug(out_runtime, "user:%lld select role, role switch to:%lld", get_user_iid(), roleid);
	}
	else if (get_role_iid() != 0)
	{
		if (cur_state_ == UserState::UserState_RoleDetailLoading)
		{
			//加载中
			return;
		}

		//角色已经加载
		if (cur_state_ == UserState::UserState_Ready)
		{
			logDebug(out_runtime, "user:%lld select role, role alreay in memory. role:%lld", get_user_iid(), roleid);

			db_role_selected_done();
			return;
		}
	}
	
	//设置当前role
	this->set_role_iid(roleid);
	s_head_.set_role_iid(roleid);

	if (this->sync_all())
	{
		db_role_selected_done();

		logDebug(out_runtime, "user:%lld select role, role load from redis. role:%lld", get_user_iid(), roleid);
	}
	else
	{
		cur_state_ = UserState::UserState_RoleDetailLoading;

		BaseDBCmd* pcmd = new LoadUserInfoCmd( s_head_, false, owner_);
		dbsStore->post_db_cmd(pcmd);
	}
}

void LobbyUser::on_ls_rolelocsave_ntf(S_INT_64 roleid, const GLoc3D& loc)
{
	if (!is_user_ready())
		return;

	if (role_iid_ != roleid)
		return;

	roles_data_.role_update_loc(get_user_iid(), roleid, loc);
	this->role_move_to(loc);
}

void LobbyUser::db_role_selected_done()
{
	cur_state_ = UserState::UserState_Ready;

	PRO::User_RoleSelect_ack* ack = new PRO::User_RoleSelect_ack();

	ack->set_role_iid(role_iid_);
	ack->set_result(0);
	//获取loc,定位世界坐标
	GLoc3D loc;
	roles_data_.get_role_loc(role_iid_, loc);
	PRO::Location3D* pos = ack->mutable_loc();
	GLoc3D::copy_to(loc, pos);

	send_to_gate(ack);

	notify_roledetail_to_user();
}

void LobbyUser::db_sync_load_done()
{
	cur_state_ = UserState::UserState_Ready;
}

void LobbyUser::notify_roledetail_to_user()
{
	PRO::User_RoleDetailA_ntf * na = new PRO::User_RoleDetailA_ntf();

	na->set_allocated_home(home_data_.clone_data<PRO::DBUserHome>());
	na->set_allocated_homeitems(building_data_.clone_data<PRO::DBUserHomeStructure>());

	send_to_gate( na);

	PRO::User_RoleDetailB_ntf* nb = new PRO::User_RoleDetailB_ntf();

	nb->set_allocated_pets(pet_data_.clone_data<PRO::DBUserPets>());

	send_to_gate(nb);

	PRO::User_RoleDetailEnd_ntf *en = new PRO::User_RoleDetailEnd_ntf();

	send_to_gate(en);
}
