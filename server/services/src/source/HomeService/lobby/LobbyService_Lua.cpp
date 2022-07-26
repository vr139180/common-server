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
