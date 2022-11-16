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

#include "lobby/LobbyService.h"

#include <taskLib/service/TaskResultLua.h>

#include "HomeServiceApp.h"

void LobbyService::init_luacontext()
{
	lua_context_.init_scriptcontext();

	lua_State* l = lua_context_.get_luastate();

	TaskResultLua::bind_luacontext(l);
	LobbyService::bind_luacontext(l);
	LobbyUser::bind_luacontext(l);
}

void LobbyService::uninit_luacontext()
{
	lua_context_.uninit_scriptcontext();
}

void LobbyService::bind_luacontext(lua_State* l)
{
	using namespace luabridge;

	getGlobalNamespace(l)
		.beginNamespace("tasks")
		.beginClass <LobbyService>("LobbyService")
		.addConstructor <void(*) (void)>()
		.endClass()
		.endNamespace();
}
