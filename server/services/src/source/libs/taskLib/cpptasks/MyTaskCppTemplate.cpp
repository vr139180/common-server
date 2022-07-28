#include <taskLib/cpptasks/MyTaskCppTemplate.h>

MyTaskCppTemplate::MyTaskCppTemplate():ITaskImplCpp()
{
}

bool MyTaskCppTemplate::can_gettask_check(ITaskContext* tc, CPPObjectiveParams* params)
{
	S_INT_32 lv = 0;
	if (!params->intval_param("lv", lv))
		return false;

	return tc->get_userenv()->get_role_level() >= lv;
}

S_INT_32 MyTaskCppTemplate::gettask_confirm(ITaskContext* tc, CPPObjectiveParams* params)
{
	return 0;
}

S_INT_32 MyTaskCppTemplate::submit_task(ITaskContext* tc, CPPObjectiveParams* params)
{
	S_INT_32 lv = 0;
	if (!params->intval_param("lv", lv))
		return 2; //system error

	if (tc->get_userenv()->get_role_level() >= lv)
		return 0;
	return 3; //condition error
}
