#include "cmsLib/lua/ScriptContext.h"

#include "cmsLib/Log.h"

#include <sstream>
#include <iosfwd>

ScriptContext::ScriptContext():
lua_state_( nullptr)
{
}

ScriptContext::~ScriptContext()
{
}

int ScriptContext::pcall_callback_fun( lua_State* L)
{
	lua_Debug d;
	lua_getstack( L, 1, &d);
	lua_getinfo(L, "Sln", &d);

	NS_STL::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	
	NS_STL::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if( d.name != 0)
	{
		msg << "(" << d.namewhat << " " << d.name << ")";
	}

	msg << " " << err;
	lua_pushstring(L, msg.str().c_str());

	return 1;
}

bool ScriptContext::init_scriptcontext()
{
	if( lua_state_)
		return true;

	lua_state_ = luaL_newstate();
	if( lua_state_ == nullptr)
		return false;

	luaL_openlibs( lua_state_);

	if( !regist_bindclass())
		return false;

	return true;
}

void ScriptContext::uninit_scriptcontext()
{
	if( lua_state_)
		lua_close( lua_state_);
	lua_state_ =nullptr;
}

bool ScriptContext::run_script( const char* cmd)
{
	int ret =LUA_ERRRUN;

	if( lua_state_ == nullptr)
		return false;

	ret = luaL_dostring(lua_state_, cmd);

	return ret == LUA_OK;
}

bool ScriptContext::run_globalfunction( const char* fun)
{
	if( lua_state_ == 0 || fun == 0 || strlen( fun) == 0)
		return false;

	try{
		lua_getglobal( lua_state_, fun);
		if( lua_isfunction( lua_state_, -1) == false)
		{
			lua_pop( lua_state_, 1);
			return false;
		}

		return lua_pcall( lua_state_, 0, LUA_MULTRET, 0) == LUA_OK;
	}
	catch( ...){
		return false;
	}
}

void ScriptContext::remove_4_context( const char* id)
{
	if( lua_state_ == 0 || id == 0 || strlen( id) == 0)
		return;

	luabridge::push(lua_state_, LUA_TNIL);
	lua_setglobal(lua_state_, id);
}

void ScriptContext::do_gc()
{
	if( lua_state_ == 0)
		return;

	lua_gc( lua_state_, LUA_GCCOLLECT, 0);
}

int ScriptContext::get_luamemsize()
{
	if( lua_state_ == 0)
		return -1;

	return lua_gc( lua_state_, LUA_GCCOUNT, 0);
}
