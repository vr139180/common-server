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

#include <cmsLib/util/ShareUtil.h>
#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>

#include <gameLib/LogExt.h>
#include <taskLib/TaskFunc_Defined.h>

#include "HomeServiceApp.h"

void LobbyUser::bind_luacontext(lua_State* l)
{
	using namespace luabridge;

	getGlobalNamespace(l)
		.beginNamespace("tasks")
		.beginClass <LobbyUser>("LobbyUser")
		.addConstructor <void(*) (void)>()
		.addFunction( task::FN_get_role_level, (int (LobbyUser::*)(void))&LobbyUser::get_role_level)
		.addFunction( task::FN_get_bag_itemnum, (int (LobbyUser::*)(S_INT_64))&LobbyUser::get_bag_itemnum)
		.endClass()
		.endNamespace();
}

std::string LobbyUser::get_luaojb_name()
{
	return ShareUtil::str_format("r:%lld", this->role_iid_);
}

//---------------------task xml objective 实现了部分lua的功能，函数名称和lua保持一致-----------------------------
bool LobbyUser::run_xmlobjective(const std::string& fn, XmlObjectiveParams* params, S_INT_64& ret)
{
	if (fn == task::FN_get_role_level)
	{
		ret = this->get_role_level();
		return true;
	}
	else if (fn == task::FN_get_bag_itemnum)
	{
		S_INT_64 itemid = 0;
		if (!params->int64val_param(1, itemid))
			return false;

		ret = this->get_bag_itemnum( itemid);
		return true;
	}

	return false;
}
