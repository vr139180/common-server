#include <taskLib/UserTasksResolver.h>

#include <gameLib/LogExt.h>

#include <taskLib/TaskMetaHome.h>

UserTasksResolver::UserTasksResolver():user_dataenv_( 0)
, global_dataenv_( 0), data_cb_(0)
{
}

UserTasksResolver::~UserTasksResolver()
{
	this->user_dataenv_ = 0;
	this->global_dataenv_ = 0;
	this->data_cb_ = 0;

	this->release();
}

void UserTasksResolver::release()
{
	processing_tasks_.clear();
	wait_accept_tasks_.clear();

	for (TASKGROUPCELLRT_MAP::iterator iter = procssing_groups_.begin(); iter != procssing_groups_.end(); ++iter)
	{
		delete iter->second;
	}
	procssing_groups_.clear();
}

void UserTasksResolver::init_env(IGlobalDataEnv* genv, IUserDataEnv* uenv, ITaskDataUpdateCB* cb)
{
	this->global_dataenv_ = genv;
	this->user_dataenv_ = uenv;
	this->data_cb_ = cb;
}

TaskGroupCellRT* UserTasksResolver::get_taskgroup_byiid(S_INT_32 iid)
{
	TASKGROUPCELLRT_MAP::iterator fiter = procssing_groups_.find(iid);
	if (fiter == procssing_groups_.end())
		return 0;
	return fiter->second;
}

void UserTasksResolver::init_taskresolver(PRO::DBUserTaskGroups& tgroup, PRO::DBUserTasks& tasks)
{
	for (int ii = 0; ii < tgroup.groups_size(); ++ii)
	{
		const PRO::DBUserTaskGroup& tg = tgroup.groups(ii);
		TaskGroupMeta* pGroup = TaskMetaHome::instance().get_taskgroup_by_iid( tg.task_group());
		if (pGroup == 0)
		{
			logError(out_sys, "task-system role:%lld task group:%d not finded", tg.role_iid(), tg.task_group());
			continue;
		}

		TaskGroupCellRT *rt = TaskGroupCellRT::build_taskgroup_rt(tg, pGroup);
		if (rt == 0)
		{
			logError(out_sys, "task-system role:%lld task group:%d build failed", tg.role_iid(), tg.task_group());
			continue;
		}

		this->procssing_groups_[rt->get_groupiid()] = rt;
	}

	for (int ii = 0; ii < tasks.items_size(); ++ii)
	{
		const PRO::DBUserTaskItem& ti = tasks.items(ii);

		TaskGroupCellRT *rt = get_taskgroup_byiid(ti.task_group());
		if (rt == 0)
		{
			logError(out_sys, "task-system role:%lld task item:%lid build failed", ti.role_iid(), ti.iid());
			continue;
		}

		TaskCellRT* tc = rt->init_processing_task(ti);
		if (tc == 0)
		{
			logError(out_sys, "task-system role:%lld task item:%lid build failed", ti.role_iid(), ti.iid());
			continue;
		}

		if (tc->is_processing())
			this->processing_tasks_[tc->get_taskiid()] = tc;
	}

	//build wait tasks
	for (TASKGROUPCELLRT_MAP::iterator iter = procssing_groups_.begin(); iter != procssing_groups_.end(); ++iter)
	{
		TaskGroupCellRT* rt = iter->second;

		//初始化，未触发的任务
		std::list<TaskCellRT*> ts;
		rt->init_wait_tasks(ts);

		for (std::list<TaskCellRT*>::iterator iter = ts.begin(); iter != ts.end(); ++iter)
		{
			TaskCellRT* r = (*iter);
			this->wait_accept_tasks_[r->get_taskiid()] = r;
		}

		forward_taskgroup(rt);
	}

	//处理boost类型的任务线
	const std::vector<TaskGroupMeta*>& bsts = TaskMetaHome::instance().get_boost_groups();
	for (int ii = 0; ii < bsts.size(); ++ii)
	{
		TaskGroupMeta* pGroup = bsts[ii];
		if (get_taskgroup_byiid(pGroup->get_iid()) != 0)
			continue;
		//check finish taskgroup
		if (user_dataenv_->is_end_of_taskgroup(pGroup->get_iid()))
			continue;

		//boost task group
		trigger_new_taskgroup(pGroup);
	}
}

void UserTasksResolver::trigger_new_taskgroup(TaskGroupMeta* pGroup)
{
	S_INT_64 giid = global_dataenv_->new_taskgroup_iid();
	TaskGroupCellRT *rt = TaskGroupCellRT::new_taskgroup_rt( giid, pGroup);
	this->procssing_groups_[rt->get_groupiid()] = rt;

	std::list<TaskCellRT*> ts;
	std::vector<int> gs;
	rt->goto_nextgroupcell(this, ts, gs, true);

	//notify to save new taskgroup data
	data_cb_->notify_new_taskgroup(rt);

	for (std::list<TaskCellRT*>::iterator iter = ts.begin(); iter != ts.end(); ++iter)
	{
		TaskCellRT* r = (*iter);
		this->wait_accept_tasks_[r->get_taskiid()] = r;
	}

	for (int ii = 0; ii < gs.size(); ++ii)
	{
		TaskGroupMeta* pGroup = TaskMetaHome::instance().get_taskgroup_by_iid(gs[ii]);
		if (pGroup == 0)
		{
			logError(out_sys, "task-system task group:%d not finded", gs[ii]);
			continue;
		}

		trigger_new_taskgroup(pGroup);
	}
}

void UserTasksResolver::forward_taskgroup(TaskGroupCellRT* rt)
{
	if (!rt->forward_groupcell())
		return;

	std::list<TaskCellRT*> ts;
	std::vector<int> gs;
	if (!rt->goto_nextgroupcell(this, ts, gs, false))
		return;

	//通知数据修改
	data_cb_->notify_forward_nextcell_taskgroup(rt);

	for (std::list<TaskCellRT*>::iterator iter = ts.begin(); iter != ts.end(); ++iter)
	{
		TaskCellRT* r = (*iter);
		this->wait_accept_tasks_[r->get_taskiid()] = r;
	}

	for (int ii = 0; ii < gs.size(); ++ii)
	{
		TaskGroupMeta* pGroup = TaskMetaHome::instance().get_taskgroup_by_iid(gs[ii]);
		if (pGroup == 0)
		{
			logError(out_sys, "task-system task group:%d not finded", gs[ii]);
			continue;
		}

		trigger_new_taskgroup(pGroup);
	}
}

void UserTasksResolver::on_roleinfo_change()
{

}

void UserTasksResolver::on_bag_change()
{

}

void UserTasksResolver::on_building_change()
{

}
