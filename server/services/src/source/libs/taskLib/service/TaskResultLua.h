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

#ifndef __TASKRESULTLUA_H__
#define __TASKRESULTLUA_H__

#include <cmsLib/core_type.h>
#include <cmsLib/lua/ScriptContext.h>

class TaskResultLua
{
public:
	static void bind_luacontext(lua_State* l);

	TaskResultLua();

	bool		is_failed() { return ret_ < 0; }
	S_INT_32	get_result() const { return ret_; }
	void		set_result(S_INT_32 v) { ret_ = v; }

private:
	S_INT_32	ret_;
};

#endif //__TASKRESULTLUA_H__
