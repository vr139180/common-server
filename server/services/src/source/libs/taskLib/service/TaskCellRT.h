#ifndef __TASKCELLRT_H__
#define __TASKCELLRT_H__

#include <boost/unordered_map.hpp>
#include <gameLib/protobuf/Proto_all.h>

#include <taskLib/meta/TaskMeta.h>
#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/ITaskImplCpp.h>

class TaskGroupCellRT;

class TaskCellRT
{
	typedef boost::unordered_map<std::string, S_INT_64>	TASK_DATA;
public:
	static TaskCellRT* build_taskrt(const PRO::DBUserTaskItem& item, TaskMetaBase* pmeta, TaskGroupCellRT* p);
	static TaskCellRT* new_task_rt(TaskMetaBase* pmeta, TaskGroupCellRT* p);

	TaskCellRT();

	bool is_waiting() { return true; }
	bool is_processing() { return qstate_ == PRO::TASKSTATE_ACCEPT || qstate_ == PRO::TASKSTATE_REACCEPT; }
	bool is_submited() { return qstate_ == PRO::TASKSTATE_SUBMIT; }
	bool is_end() { return qstate_ > PRO::TASKSTATE_SUBMIT; }

	S_INT_64 get_iid() { return iid_; }
	S_INT_32 get_taskiid() { return task_iid_; }
	PRO::TASK_STATE get_qstate() { return qstate_; }
	bool is_cycle_task() { return b_cycle_task_; }
	S_INT_32 get_cycle_num() { return cycle_num_; }

	bool is_task_cpp_valide() { return task_cpp_.get() != 0; }

	const TASK_DATA& get_taskdatas() { return task_datas_; }

	TaskGroupCellRT* owner_taskgroup() { return parent_; }

public:
	//是否能取得这个任务
	bool can_get_task(ITaskContext* tc);
	void user_gettask(S_INT_64 iid, ITaskContext* tc);

	S_INT_32 submit_task(ITaskContext* tc);

protected:
	//检测xml配置条件
	bool check_xml_conditions(ITaskContext* tc, TaskXmlCondition* pcond);

protected:
	S_INT_64	iid_;

	S_INT_32		task_iid_;
	TaskMetaBase	*task_meta_;

	std::shared_ptr<ITaskImplCpp>	task_cpp_;

	TASK_DATA		task_datas_;
	PRO::TASK_STATE	qstate_;

	bool		b_cycle_task_;
	S_INT_32	cycle_num_;

	TaskGroupCellRT	*parent_;
};

#endif //__TASKCELLRT_H__
