#ifndef __TASKGROUPCELLRT_H__
#define __TASKGROUPCELLRT_H__

#include <boost/unordered_map.hpp>
#include <gameLib/protobuf/Proto_all.h>
#include <taskLib/meta/TaskGroupMeta.h>
#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/TaskCellRT.h>

class TaskGroupCellRT
{
	typedef boost::unordered_map<S_INT_32, PRO::TASK_STATE>	CELL_DATA;
	typedef boost::unordered_map<S_INT_32, TaskCellRT*>		TASK_RT;
public:
	static TaskGroupCellRT* build_taskgroup_rt(const PRO::DBUserTaskGroup& dat, TaskGroupMeta* pGroup);
	//构建新的任务线
	//未设置iid
	static TaskGroupCellRT* new_taskgroup_rt(S_INT_64 giid, TaskGroupMeta* pGroup);

	TaskGroupCellRT();
	~TaskGroupCellRT();
	S_INT_64 get_iid() { return giid_; }
	S_INT_32 get_groupiid() { return group_iid_; }
	S_INT_32 get_groupcell_iid() { return cell_iid_; }
	PRO::TASK_GROUP_STATE get_gstate() { return gstate_; }
	const CELL_DATA& get_celldata() { return datas_; }

	TaskCellRT* init_processing_task(const PRO::DBUserTaskItem& item);
	void init_wait_tasks(std::list<TaskCellRT*>& ts);

	TaskCellRT* get_task_byiid(S_INT_32 iid);

	//推进当前group cell, return true:进入下个节点， false:维持当前节点
	bool forward_groupcell();
	//进入下一个group cell，只触发一次
	//true, waits: 返回可接的任务 groups:返回触发的任务线 start:第一次初始化
	bool goto_nextgroupcell(ITaskContext* tc, std::list<TaskCellRT*>& waits, std::vector<int>& groups, bool start = false);
	//任务线是否结束
	bool is_groupend();

protected:
	void release();

	//计算转移条件
	bool check_conditions(ITaskContext* tc, TaskXmlCondition* pcond);

protected:
	S_INT_64				giid_;

	S_INT_32				group_iid_;
	TaskGroupMeta*			group_meta_;
	PRO::TASK_GROUP_STATE	gstate_;

	S_INT_32			cell_iid_;
	TaskGroupCellMeta*	group_cell_meta_;
	CELL_DATA			datas_;

	//都是使用的配置文件中的task_iid
	TASK_RT		processing_tasks_;
	TASK_RT		wait_tasks_;
	TASK_RT		all_tasks_;
};

#endif //__TASKGROUPCELLRT_H__
