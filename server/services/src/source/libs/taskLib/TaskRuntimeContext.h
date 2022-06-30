#ifndef __TASKRUNTIMECONTEXT_H__
#define __TASKRUNTIMECONTEXT_H__

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/ITaskResTrigger.h>

//����ϵͳ�����ģ��
class TaskRuntimeContext : public ITaskResTrigger
{
public:
	TaskRuntimeContext();

public:
	//--------------------------ITaskResTrigger---------------------
	virtual void on_roleinfo_change();
	virtual void on_bag_change();
	virtual void on_building_change();

private:
};

#endif //__TASKRUNTIMECONTEXT_H__
