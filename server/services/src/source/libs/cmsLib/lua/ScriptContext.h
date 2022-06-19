#ifndef __SCRIPTCONTEXT__H__
#define __SCRIPTCONTEXT__H__

#include <string>

#include <lua.hpp>

#include <cmsLib/prolib/core_type.h>
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

protected:
	bool regist_bindclass();

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
