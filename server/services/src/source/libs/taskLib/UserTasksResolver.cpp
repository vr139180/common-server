// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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

void UserTasksResolver::reset_resolver()
{
	release();
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
			logError(out_runtime, "task-system role:%lld task group:%d not finded", tg.role_iid(), tg.task_group());
			continue;
		}

		TaskGroupCellRT *rt = TaskGroupCellRT::build_taskgroup_rt(tg, pGroup);
		if (rt == 0)
		{
			logError(out_runtime, "task-system role:%lld task group:%d build failed", tg.role_iid(), tg.task_group());
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
			logError(out_runtime, "task-system role:%lld task item:%lid build failed", ti.role_iid(), ti.iid());
			continue;
		}

		TaskCellRT* tc = rt->init_processing_task(ti);
		if (tc == 0)
		{
			logError(out_runtime, "task-system role:%lld task item:%lid build failed", ti.role_iid(), ti.iid());
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
			logError(out_runtime, "task-system task group:%d not finded", gs[ii]);
			continue;
		}

		trigger_new_taskgroup(pGroup);
	}

	this->forward_taskgroup(rt);
}

void UserTasksResolver::forward_taskgroup(TaskGroupCellRT* rt)
{
	if (!rt->forward_groupcell())
		return;

	std::list<TaskCellRT*> ts;
	std::vector<int> gs;
	if (!rt->goto_nextgroupcell(this, ts, gs, false))
		return;

	if (rt->is_groupend())
	{
		this->taskgroup_end(rt);
		return;
	}

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
			logError(out_runtime, "task-system task group:%d not finded", gs[ii]);
			continue;
		}

		trigger_new_taskgroup(pGroup);
	}

	//loop forward
	this->forward_taskgroup(rt);
}

void UserTasksResolver::taskgroup_end(TaskGroupCellRT* pGroup)
{
	data_cb_->notify_taskgroup_end(pGroup);

	//destroy taskgroup object
	TASKGROUPCELLRT_MAP::iterator fiter = procssing_groups_.find(pGroup->get_groupiid());
	if (fiter != procssing_groups_.end())
	{
		delete fiter->second;
		procssing_groups_.erase(fiter);
	}
}

bool UserTasksResolver::get_task_from_waitlist(S_INT_32 taskid)
{
	TASKCELLRT_MAP::iterator fiter = wait_accept_tasks_.find(taskid);
	if (fiter == wait_accept_tasks_.end())
		return false;

	TaskCellRT* tc = fiter->second;
	if (!tc->can_get_task(this))
		return false;

	S_INT_64 tid = global_dataenv_->new_task_iid();
	tc->user_gettask(tid, this);

	wait_accept_tasks_.erase(fiter);
	processing_tasks_[tc->get_taskiid()] = tc;

	TaskGroupCellRT* tgroup = tc->owner_taskgroup();
	tgroup->user_gettask(taskid);

	data_cb_->notify_user_gettask(tgroup, tc);

	return true;
}

S_INT_32 UserTasksResolver::submit_task(S_INT_32 taskid)
{
	TASKCELLRT_MAP::iterator fiter = processing_tasks_.find(taskid);
	if (fiter == processing_tasks_.end())
		return 1;	//task not exist

	TaskCellRT* tc = fiter->second;
	TaskGroupCellRT* pGroup = tc->owner_taskgroup();
	S_INT_32 ret = pGroup->user_submittask(tc, this);

	//success
	if (ret == 0)
	{
		processing_tasks_.erase(fiter);

		data_cb_->notify_end_task(pGroup, tc);

		TaskRewardMeta* rewardMeta = tc->get_rewards();
		if (rewardMeta != 0 && rewardMeta->have_rewards())
		{
			data_cb_->notify_get_taskreward(tc, rewardMeta);
		}

		//推进节点
		this->forward_taskgroup(pGroup);
	}

	return ret;
}

S_INT_32 UserTasksResolver::giveup_task(S_INT_32 taskid)
{
	return 1;
}