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

#include "HomeServiceApp.h"

void LobbyUser::on_db_rolelist_update( UserRoles& from)
{
	this->roles_data_.reset_data();
	this->roles_data_.data_copyall(from);

	//update redis
	RedisClient *rdv = svrApp.get_redisclient();
	roles_data_.update_redis_cache( get_user_iid(), rdv);

	//notify user
	PRO::User_RoleList_ack *ack = new PRO::User_RoleList_ack();
	ack->mutable_roles()->CopyFrom(roles_data_.roles_data_);

	send_to_gate(ack);

	logDebug(out_runtime, "send rolelist:%d to user....", roles_data_.roles_data_.roles_size());
}
