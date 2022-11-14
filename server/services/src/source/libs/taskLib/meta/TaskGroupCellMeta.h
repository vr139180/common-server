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

#ifndef __TASKGROUPCELLMETA_H__
#define __TASKGROUPCELLMETA_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include <cmsLib/util/XmlUtil.h>
#include <taskLib/meta/TaskMeta.h>
#include <taskLib/meta/ConditionsMeta.h>

class TaskGroupMeta;

class TaskGroupCellMeta
{
	typedef boost::unordered_map<int, TaskMetaBase*>	TASKS_MAP;
private:
	TaskGroupCellMeta();
	void release();

public:
	static TaskGroupCellMeta* load_groupcell(tinyxml2::XMLElement* e, TaskGroupMeta* p);
	~TaskGroupCellMeta();

public:
	int get_iid() { return cell_iid_; }
	int get_taskgroup_iid();
	bool have_tasks() { return tasks_.size() > 0; }
	bool have_taskgroups() { return trigger_taskgroups_.size() > 0; }

	std::vector<int> get_trigger_taskgroups() { return trigger_taskgroups_; }
	const TASKS_MAP& get_tasks() { return tasks_; }

	TaskMetaBase* get_task_byiid(int iid);

	const boost::unordered_map<IConditionsMeta*, int>& get_next_conditions() { return next_conditions_; }
	int get_default_next() { return next_default_celliid_; }

private:
	int			cell_iid_;

	//触发的并行任务
	TASKS_MAP			tasks_;
	//触发的任务线
	std::vector<int>	trigger_taskgroups_;

	//下一个缺省节点
	int			next_default_celliid_;
	boost::unordered_map<IConditionsMeta*, int>	next_conditions_;

	TaskGroupMeta*	parent_;
};

#endif //__TASKGROUPCELLMETA_H__
