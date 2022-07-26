#ifndef __TASKRESULTLUA_H__
#define __TASKRESULTLUA_H__

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/lua/ScriptContext.h>

class TaskResultLua
{
public:
	static void bind_luacontext(lua_State* l);

	TaskResultLua();

	bool		is_failed() { return ret_ < 0; }
	S_INT_32	get_result() { return ret_; }
	void		set_result(S_INT_32 v) { ret_ = v; }

private:
	S_INT_32	ret_;
};

#endif //__TASKRESULTLUA_H__
