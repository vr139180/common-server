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

#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>

#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

void LobbyUser::task_data_loaded()
{
	task_resolver_.init_taskresolver( task_data_.task_groups_data_, task_data_.tasks_data_);
}

bool LobbyUser::is_end_of_taskgroup(S_INT_32 gid)
{
	return task_data_.is_taskgroup_end( gid);
}

int LobbyUser::get_role_level()
{
	return base_data_.get_levels();
}

int LobbyUser::get_bag_itemnum(S_INT_64 itemiid)
{
	return 1;
}
