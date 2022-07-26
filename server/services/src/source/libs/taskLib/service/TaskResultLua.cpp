#include <taskLib/service/TaskResultLua.h>

void TaskResultLua::bind_luacontext(lua_State* l)
{
	using namespace luabridge;

	getGlobalNamespace(l)
		.beginNamespace("tasks")
		.beginClass <TaskResultLua>("TaskReultLua")
		.addConstructor <void(*) (void)>()
		.addFunction("result", (void (TaskResultLua::*)(S_INT_32))&TaskResultLua::set_result)
		.endClass()
		.endNamespace();
}

TaskResultLua::TaskResultLua():ret_(-1)
{
}
