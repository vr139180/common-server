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

#ifndef __TASKMETAHOME_H__
#define __TASKMETAHOME_H__

#include <boost/unordered_map.hpp>
#include <taskLib/meta/TaskGroupMeta.h>

class TaskMetaHome
{
	typedef boost::unordered_map<int, TaskGroupMeta*>	TASKGROUP_MAP;
private:
	TaskMetaHome() {}

	void release();

public:
	static TaskMetaHome& instance();
	~TaskMetaHome();

	//加载任务配置信息
	bool load_taskmetas();

	TaskGroupMeta* get_taskgroup_by_iid(int gid);
	const std::vector<TaskGroupMeta*>& get_boost_groups() { return boost_groups_; }

private:
	std::vector<TaskGroupMeta*>	boost_groups_;
	TASKGROUP_MAP	main_tasks_;
	TASKGROUP_MAP	branch_tasks_;
	TASKGROUP_MAP	all_groups_;
};

#endif //__TASKMETAHOME_H__
