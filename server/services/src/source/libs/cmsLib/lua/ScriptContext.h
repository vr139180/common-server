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

#ifndef __SCRIPTCONTEXT__H__
#define __SCRIPTCONTEXT__H__

#include <string>

#include <lua.hpp>

#include <cmsLib/core_type.h>
#include "LuaBridge/LuaBridge.h"

class ScriptContext
{
public:
	ScriptContext(void);
	virtual ~ScriptContext(void);

public:
	bool init_scriptcontext();

	void uninit_scriptcontext();

	lua_State* get_luastate(){ return lua_state_;}

	bool run_script( const char* cmd);

	bool run_globalfunction( const char* fun);

	template<class X>
	bool regist_2_context( const char* id, X *p);

	void remove_4_context( const char* id);

	static int pcall_callback_fun( lua_State* L);

public:
	//-----------------------------debug function---------------------------------
	void do_gc();

	int get_luamemsize();

private:
	lua_State*	lua_state_;
};

template<class X>
bool ScriptContext::regist_2_context( const char* id, X *p)
{
	if( lua_state_ == 0 || id == 0 || strlen( id) == 0)
		return false;

	try{
		luabridge::push<X*>(lua_state_, p);
		lua_setglobal(lua_state_, id);
	}
	catch(...){
		return false;
	}

	return true;
}

template<class T>
class ScriptGlobalParamGuard
{
public:
	ScriptGlobalParamGuard( ScriptContext* pcontext, const char* id, T* val);
	~ScriptGlobalParamGuard();

private:
	ScriptContext*	script_;
	const char*	iid_;
};

template<class T>
ScriptGlobalParamGuard<T>::ScriptGlobalParamGuard( ScriptContext* pcontext, const char* id, T* val):
script_( pcontext),
iid_( id)
{
	script_->regist_2_context<T>( iid_, val);
}

template<class T>
ScriptGlobalParamGuard<T>::~ScriptGlobalParamGuard()
{
	script_->remove_4_context( iid_);
}

#endif	//__SCRIPTCONTEXT__H__
