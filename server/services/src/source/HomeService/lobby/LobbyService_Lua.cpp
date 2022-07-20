#include "lobby/LobbyService.h"

#include "HomeServiceApp.h"

void LobbyService::init_luacontext()
{
	lua_context_.init_scriptcontext();

	lua_State* l = lua_context_.get_luastate();
	LobbyUser::bind_luacontext(l);
}

void LobbyService::uninit_luacontext()
{
	lua_context_.uninit_scriptcontext();
}