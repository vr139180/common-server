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

#include <taskLib/service/TaskGroupCellRT.h>

#include <gameLib/LogExt.h>

TaskGroupCellRT::TaskGroupCellRT():group_meta_( 0)
,group_cell_meta_( 0)
{
}

TaskGroupCellRT::~TaskGroupCellRT()
{
	group_cell_meta_ = 0;
	group_meta_ = 0;

	release();
}

void TaskGroupCellRT::release()
{
	processing_tasks_.clear();
	wait_tasks_.clear();

	for (TASK_RT::iterator iter = all_tasks_.begin(); iter != all_tasks_.end(); ++iter)
	{
		delete iter->second;
	}
	all_tasks_.clear();
}

TaskGroupCellRT* TaskGroupCellRT::build_taskgroup_rt(const PRO::DBUserTaskGroup& dat, TaskGroupMeta* pGroup)
{
	TaskGroupCellRT *rt = new TaskGroupCellRT();
	std::unique_ptr< TaskGroupCellRT> ptr(rt);

	rt->group_iid_ = dat.task_group();
	rt->group_meta_ = pGroup;

	rt->cell_iid_ = dat.group_cell();
	rt->group_cell_meta_ = pGroup->get_groupcell_byiid( rt->cell_iid_);
	if (rt->group_cell_meta_ == 0)
		return 0;

	rt->giid_ = dat.iid();
	rt->gstate_ = (PRO::TASK_GROUP_STATE)dat.gstate();

	for (google::protobuf::Map<S_INT_32,S_INT_32>::const_iterator iter = dat.cell_data().datas().begin(); iter != dat.cell_data().datas().end(); ++iter)
	{
		rt->datas_[iter->first] = (PRO::TASK_STATE)iter->second;
	}
	
	return ptr.release();
}

TaskGroupCellRT* TaskGroupCellRT::new_taskgroup_rt(S_INT_64 giid, TaskGroupMeta* pGroup)
{
	TaskGroupCellRT *rt = new TaskGroupCellRT();
	std::unique_ptr< TaskGroupCellRT> ptr(rt);

	rt->group_iid_ = pGroup->get_iid();
	rt->group_meta_ = pGroup;

	rt->group_cell_meta_ = pGroup->get_start_cell();
	rt->cell_iid_ = rt->group_cell_meta_->get_iid();

	rt->giid_ = giid;
	rt->gstate_ = PRO::TASKGROUP_ACCEPT;

	const boost::unordered_map<int, TaskMetaBase*>& tss = rt->group_cell_meta_->get_tasks();
	for (boost::unordered_map<int, TaskMetaBase*>::const_iterator citer = tss.begin(); citer != tss.end(); ++citer)
	{
		rt->datas_[citer->first] = PRO::TASKSTATE_WAIT;
	}

	return ptr.release();
}

TaskCellRT* TaskGroupCellRT::get_task_byiid(S_INT_32 iid)
{
	TASK_RT::iterator fiter = all_tasks_.find(iid);
	if (fiter == all_tasks_.end())
		return 0;
	return fiter->second;
}

TaskCellRT* TaskGroupCellRT::init_processing_task(const PRO::DBUserTaskItem& item)
{
	TaskMetaBase* pMeta = group_cell_meta_->get_task_byiid(item.task_iid());
	if (pMeta == 0)
		return 0;

	TaskCellRT *rt = TaskCellRT::build_taskrt(item, pMeta, this);
	if (rt == 0)
		return rt;

	processing_tasks_[rt->get_taskiid()] = rt;

	return rt;
}

void TaskGroupCellRT::init_wait_tasks(std::list<TaskCellRT*>& tcs)
{
	//过滤完成的和正在处理的
	for (CELL_DATA::iterator iter = datas_.begin(); iter != datas_.end(); ++iter)
	{
		PRO::TASK_STATE ts = iter->second;
		if (ts != PRO::TASKSTATE_WAIT)
			continue;

		TaskMetaBase* pMeta = group_cell_meta_->get_task_byiid(iter->first);
		if (pMeta == 0)
		{
			logError(out_runtime, "task system - ");
			continue;
		}

		TaskCellRT *rt = TaskCellRT::new_task_rt( pMeta, this);
		wait_tasks_[rt->get_taskiid()] = rt;

		tcs.push_back(rt);
	}
}

void TaskGroupCellRT::release_groupcell_tasks()
{
	processing_tasks_.clear();
	wait_tasks_.clear();

	for (TASK_RT::iterator iter = all_tasks_.begin(); iter != all_tasks_.end(); ++iter)
	{
		delete iter->second;
	}
	all_tasks_.clear();
}

bool TaskGroupCellRT::forward_groupcell()
{
	if (this->is_groupend())
		return false;

	//检测是否所有的任务都完成了
	for (CELL_DATA::iterator iter = datas_.begin(); iter != datas_.end(); ++iter)
	{
		PRO::TASK_STATE ts = iter->second;
		if (ts < PRO::TASKSTATE_FAILED)
			return false;
	}

	if (processing_tasks_.size() > 0 || wait_tasks_.size() > 0)
		return false;
	
	return true;
}

bool TaskGroupCellRT::goto_nextgroupcell(ITaskContext* tc, std::list<TaskCellRT*>& waits, std::vector<int>& groups, bool start)
{
	if (group_meta_->is_end_cell(this->cell_iid_))
	{
		this->gstate_ = PRO::TASKGROUP_FINISH;
		return true;
	}

	//不是第一次启动任务线
	//需要切换到下一个节点
	if (!start)
	{
		int ncell = group_cell_meta_->get_default_next();

		const boost::unordered_map<IConditionsMeta*, int>& nextconds = group_cell_meta_->get_next_conditions();
		for (boost::unordered_map<IConditionsMeta*, int>::const_iterator iter = nextconds.begin(); iter != nextconds.end(); ++iter)
		{
			IConditionsMeta* pcm = iter->first;
			if (!pcm->is_xmlcondition())
				continue;
			ConditionsMetaXml* pxcond = dynamic_cast<ConditionsMetaXml*>(pcm);
			if (check_conditions(tc, pxcond->get_condition()))
			{
				ncell = iter->second;
				break;
			}
		}

		//next cell
		group_cell_meta_ = group_meta_->get_groupcell_byiid(ncell);
		cell_iid_ = group_cell_meta_->get_iid();

		datas_.clear();
		const boost::unordered_map<int, TaskMetaBase*>& tss = group_cell_meta_->get_tasks();
		for (boost::unordered_map<int, TaskMetaBase*>::const_iterator citer = tss.begin(); citer != tss.end(); ++citer)
		{
			datas_[citer->first] = PRO::TASKSTATE_WAIT;
		}
	}

	//释放上一个节点创建的任务
	release_groupcell_tasks();

	//初始化可接的任务
	this->init_wait_tasks(waits);
	//获取可以触发的任务线
	groups = group_cell_meta_->get_trigger_taskgroups();

	return true;
}

bool TaskGroupCellRT::is_groupend()
{
	return gstate_ > PRO::TASKGROUP_ACCEPT;
}

bool TaskGroupCellRT::check_conditions(ITaskContext* tc, TaskXmlCondition* pcond)
{
	if (pcond->is_leaf())
	{
		const std::vector<XmlObjectvie*>& objs = pcond->get_objectives();
		if (objs.size() == 0)
			return true;

		std::vector<bool> allret;
		for (size_t ii = 0; ii < objs.size(); ++ii)
		{
			XmlObjectvie* xobj = objs[ii];
			S_INT_64 retval = 0;
			if (tc->run_xmlobjective(xobj->get_funname(), xobj->get_params(), retval) && xobj->check_result(retval))
			{
				allret.push_back(true);
			}
			else
			{
				allret.push_back(false);
			}
		}

		return pcond->check_results(allret);
	}
	else
	{
		const std::vector<TaskXmlCondition*>& cconds = pcond->get_childen_conditions();
		if (cconds.size() == 0)
			return false;

		std::vector<bool> allret;
		for (size_t ii = 0; ii < cconds.size(); ++ii)
		{
			bool v = this->check_conditions(tc, cconds[ii]);
			allret.push_back(v);
		}

		return pcond->check_results(allret);
	}

	return false;
}

TaskCellRT* TaskGroupCellRT::user_gettask(S_INT_32 taskid)
{
	TASK_RT::iterator fiter = wait_tasks_.find(taskid);
	if (fiter == wait_tasks_.end())
		return 0;

	TaskCellRT* tc = fiter->second;

	//切换队列
	wait_tasks_.erase(fiter);
	processing_tasks_[tc->get_taskiid()] = tc;

	//修改cell_data状态
	datas_[tc->get_taskiid()] = PRO::TASKSTATE_ACCEPT;

	return tc;
}

//submit成功之后，并不会马上销毁对应的task,在taskgroup转向下一个cell节点时会统一销毁
S_INT_32 TaskGroupCellRT::user_submittask(TaskCellRT* tc, ITaskContext* context)
{
	TASK_RT::iterator fiter = processing_tasks_.find(tc->get_taskiid());
	if (fiter == processing_tasks_.end())
		return 1; //task not exist

	if (fiter->second != tc)
		return 2;  //system error

	S_INT_32 sret = tc->submit_task(context);
	if (sret != 0)
		return sret;

	//remove from list, but dont destroy
	processing_tasks_.erase(fiter);

	datas_[tc->get_taskiid()] = PRO::TASKSTATE_FINISH;

	return 0;
}
