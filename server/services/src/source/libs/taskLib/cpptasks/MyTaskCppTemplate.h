#ifndef __MYTASKCPPTEMPLATE_H__
#define __MYTASKCPPTEMPLATE_H__

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/ITaskImplCpp.h>

class MyTaskCppTemplate : public ITaskImplCpp
{
public:
	MyTaskCppTemplate();
	virtual ~MyTaskCppTemplate() {}

	virtual bool can_gettask_check(ITaskContext* tc, CPPObjectiveParams* params);
	virtual S_INT_32 gettask_confirm(ITaskContext* tc, CPPObjectiveParams* params);
	virtual S_INT_32 submit_task(ITaskContext* tc, CPPObjectiveParams* params);
};

#endif //__MYTASKCPPTEMPLATE_H__