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