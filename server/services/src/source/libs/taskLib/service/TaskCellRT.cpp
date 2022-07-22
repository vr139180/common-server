#include <taskLib/service/TaskCellRT.h>

#include <taskLib/service/TaskGroupCellRT.h>

TaskCellRT::TaskCellRT():task_meta_( 0)
, parent_(0), b_cycle_task_(false), cycle_num_(1)
{
}

TaskCellRT* TaskCellRT::build_taskrt(const PRO::DBUserTaskItem& item, TaskMetaBase* pmeta, TaskGroupCellRT* p)
{
	TaskCellRT *rt = new TaskCellRT();
	std::unique_ptr<TaskCellRT> ptr(rt);

	rt->iid_ = item.iid();

	rt->task_iid_ = item.task_iid();
	rt->task_meta_ = pmeta;

	rt->qstate_ = (PRO::TASK_STATE)item.qstate();
	for (google::protobuf::Map<std::string, S_INT_64>::const_iterator iter = item.task_datas().datas().begin();
		iter != item.task_datas().datas().end(); ++iter)
	{
		rt->task_datas_[iter->first] = iter->second;
	}
	
	rt->b_cycle_task_ = item.cycle_task();
	rt->cycle_num_ = item.cycle_num();

	rt->parent_ = p;

	return ptr.release();
}

TaskCellRT* TaskCellRT::new_task_rt(TaskMetaBase* pmeta, TaskGroupCellRT* p)
{
	TaskCellRT *rt = new TaskCellRT();
	std::unique_ptr<TaskCellRT> ptr(rt);

	rt->task_iid_ = pmeta->get_iid();
	rt->task_meta_ = pmeta;

	rt->qstate_ = PRO::TASKSTATE_WAIT;
	rt->b_cycle_task_ = pmeta->is_cycle_task();
	rt->cycle_num_ = pmeta->cycle_num();

	rt->parent_ = p;

	return ptr.release();
}

bool TaskCellRT::can_get_task(ITaskContext* tc)
{
	return false;
}

void TaskCellRT::user_gettask(S_INT_64 iid)
{
	this->iid_ = iid;
	this->qstate_ = PRO::TASKSTATE_ACCEPT;
	++this->cycle_num_;
}

S_INT_32 TaskCellRT::submit_task(ITaskContext* tc)
{

	return 0;
}
