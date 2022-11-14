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

#ifndef __USERTASKS_H__
#define __USERTASKS_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>
#include <gameLib/protobuf/cpp/task_system.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserTasks : public UserOperate
{
	friend class LobbyUser;

public:
	UserTasks();
	virtual ~UserTasks() {}

	virtual void reset_data();

	virtual void load_from_database1(sql::ResultSet& row);
	virtual void load_from_database2(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &task_groups_data_; }

	void data_copyall(UserTasks& from);

	bool update_redis_groupcache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_groupendcache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_taskcache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_taskendcache(S_INT_64 uid, RedisClient* rdv, bool force = false);

	bool update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force = false);

	void load_from_redis(S_INT_64 roleid, RedisClient* rdv);

	virtual bool is_modify() { return taskgroups_data_update_ || tasks_update_; }

	const PRO::DBUserTaskGroups& get_taskgroups() { return task_groups_data_; }
	const PRO::DBUserTasks& get_tasks() { return tasks_data_; }

	bool is_taskgroup_end(S_INT_32 gid);

public:

	PRO::DBUserTaskGroup* new_taskgroup();
	PRO::DBUserTaskGroup* get_taskgroup_for_update(S_INT_32 groupiid);
	PRO::DBUserTaskGroupEnd* end_taskgroup(S_INT_32 groupid, PRO::TASK_GROUP_STATE gstate);

	PRO::DBUserTaskItem* new_taskitem();
	PRO::DBUserTaskItem* get_taskitem_for_update(S_INT_32 taskid);
	PRO::DBUserTaskItem* get_taskitem(S_INT_32 taskid);
	PRO::DBUserTaskEndItem* end_taskitem(S_INT_32 taskid, PRO::TASK_STATE qstate);

protected:
	PRO::DBUserTaskGroups	task_groups_data_;
	bool					taskgroups_data_update_;

	PRO::DBUserTaskGroupEnds	task_group_ends_data_;
	bool						task_group_ends_data_update_;

	PRO::DBUserTasks		tasks_data_;
	bool					tasks_update_;

	PRO::DBUserTaskEnds		task_ends_data_;
	bool					task_ends_data_update_;

};

#endif //__USERTASKS_H__
