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

#ifndef __USERTASKSRESOLVER_H__
#define __USERTASKSRESOLVER_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/lua/ScriptContext.h>
#include <gameLib/protobuf/Proto_all.h>

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/ITaskResTrigger.h>
#include <taskLib/service/TaskCellRT.h>
#include <taskLib/service/TaskGroupCellRT.h>

//任务系统的外接模块
class UserTasksResolver : public ITaskContext
{
	typedef boost::unordered_map<S_INT_32, TaskCellRT*>			TASKCELLRT_MAP;
	typedef boost::unordered_map<S_INT_32, TaskGroupCellRT*>	TASKGROUPCELLRT_MAP;
public:
	UserTasksResolver();
	~UserTasksResolver();
	void init_env(IGlobalDataEnv* genv, IUserDataEnv* uenv, ITaskDataUpdateCB* cb);

	void reset_resolver();

	//初始化用户任务系统
	void init_taskresolver(PRO::DBUserTaskGroups& tgroup, PRO::DBUserTasks& tasks);

	TaskGroupCellRT* get_taskgroup_byiid(S_INT_32 iid);
	const TASKCELLRT_MAP& get_waittask_list() { return wait_accept_tasks_; }

public:
	//获取可执行的任务
	bool get_task_from_waitlist(S_INT_32 taskid);
	S_INT_32 submit_task(S_INT_32 taskid);
	S_INT_32 giveup_task(S_INT_32 taskid);

protected:
	void release();

	//触发新的任务线
	void trigger_new_taskgroup(TaskGroupMeta* pGroup);
	void forward_taskgroup(TaskGroupCellRT* rt);

	void taskgroup_end(TaskGroupCellRT* pGroup);

public:
	//--------------------------ITaskContext------------------------
	virtual IGlobalDataEnv* get_globalevn() { return global_dataenv_; }
	virtual IUserDataEnv* get_userenv() { return user_dataenv_; }

protected:
	//任务信息
	TASKCELLRT_MAP	wait_accept_tasks_;
	TASKCELLRT_MAP	processing_tasks_;

	TASKGROUPCELLRT_MAP	procssing_groups_;

private:
	IGlobalDataEnv*		global_dataenv_;
	IUserDataEnv*		user_dataenv_;
	ITaskDataUpdateCB*	data_cb_;
};

#endif //__USERTASKSRESOLVER_H__
