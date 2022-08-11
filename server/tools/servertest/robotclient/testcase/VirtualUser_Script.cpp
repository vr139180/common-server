#include "StdAfx.h"

#include <cmsLib/Log.h>
#include "VirtualUser.h"

bool VirtualUser::run_script( const char* cmd)
{
	cond_result_.reset();
	if( !context_.run_script( cmd))
	{
		logError( out_runtime, ">>>>>>>>>> script error:[%s]", cmd);
	}

	return cond_result_.succ;
}

void VirtualUser::InitScriptBind( lua_State* l)
{
	using namespace luabridge;

	getGlobalNamespace(l)
		.beginClass <CaseConditionResult>("CaseConditionResult")
		.addConstructor <void(*) (void)>()
		.addData("ret", &CaseConditionResult::succ, true)
		.endClass();

	getGlobalNamespace(l)
		.beginClass <VirtualUser>("VirtualUser")
		.addConstructor <void(*) (void)>()
		.addFunction("logintime", (int (VirtualUser::*)(void))&VirtualUser::logintime)
		.addFunction("islogin", (bool (VirtualUser::*)(void))&VirtualUser::islogin)
		.addFunction("haschr", (bool (VirtualUser::*)(void))&VirtualUser::has_chr)
		.addFunction("isready", (bool (VirtualUser::*)(void))&VirtualUser::is_ready)
		.endClass();

}