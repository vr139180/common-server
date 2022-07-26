#include <taskLib/cpptasks/MyTaskCppTemplate.h>

MyTaskCppTemplate::MyTaskCppTemplate():ITaskImplCpp()
{
}

bool MyTaskCppTemplate::can_gettask_check(ITaskContext* tc)
{
	return true;
}

S_INT_32 MyTaskCppTemplate::gettask_confirm(ITaskContext* tc)
{
	return 0;
}

S_INT_32 MyTaskCppTemplate::submit_task(ITaskContext* tc)
{
	return 0;
}
