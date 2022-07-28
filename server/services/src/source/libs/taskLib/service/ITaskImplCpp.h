#ifndef __ITASKIMPLCPP_H__
#define __ITASKIMPLCPP_H__

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/meta/TaskCppObjective.h>

//task��cppʵ����
class ITaskImplCpp
{
public:
	//�������ֹ���cppʵ��
	static ITaskImplCpp* build_cppimpl_from_name(const char* name);

	ITaskImplCpp() {}
	virtual ~ITaskImplCpp() {}

	//����Ƿ��ܻ�ȡ������
	virtual bool can_gettask_check(ITaskContext* tc, CPPObjectiveParams* params) = 0;
	virtual S_INT_32 gettask_confirm(ITaskContext* tc, CPPObjectiveParams* params) = 0;

	virtual S_INT_32 submit_task(ITaskContext* tc, CPPObjectiveParams* params) = 0;
};

#endif //__ITASKIMPLCPP_H__
