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

#include "lobby/user/UserTasks.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

using namespace google::protobuf;

USED_REDISKEY_USER_NS

UserTasks::UserTasks():taskgroups_data_update_(false)
, tasks_update_(false), task_group_ends_data_update_( false), task_ends_data_update_( false)
{
}

void UserTasks::reset_data()
{
	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	task_group_ends_data_.Clear();
	task_group_ends_data_update_ = false;

	tasks_data_.Clear();
	tasks_update_ = false;

	task_ends_data_.Clear();
	task_ends_data_update_ = false;
}

void UserTasks::data_copyall(UserTasks& from)
{
	data_copy_from<PRO::DBUserTaskGroups>(&from.task_groups_data_, &task_groups_data_);
	data_copy_from<PRO::DBUserTaskGroupEnds>(&from.task_group_ends_data_, &task_group_ends_data_);

	data_copy_from<PRO::DBUserTasks>(&from.tasks_data_, &tasks_data_);
	data_copy_from<PRO::DBUserTaskEnds>(&from.task_ends_data_, &task_ends_data_);
}

void UserTasks::load_from_database1(sql::ResultSet& row)
{
	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	while (row.next())
	{
		int column = 1;
		S_INT_32 gstate = row.getInt(column++);

		if (gstate == (int)PRO::TASKGROUP_ACCEPT)
		{
			PRO::DBUserTaskGroup *pitem = task_groups_data_.add_groups();

			pitem->set_gstate(gstate);
			pitem->set_ver_(row.getUInt(column++));

			pitem->set_iid(row.getInt64(column++));
			pitem->set_role_iid(row.getInt64(column++));
			pitem->set_task_group(row.getInt(column++));
			pitem->set_group_cell(row.getInt(column++));

			std::string cdata = row.getString(column++);
			pitem->mutable_cell_data()->ParseFromArray(cdata.c_str(), cdata.size());

			pitem->set_trigg_level(row.getInt(column++));
			pitem->set_createtime(row.getInt(column++));
			pitem->set_endtime(row.getInt(column++));
		}
		else
		{
			PRO::DBUserTaskGroupEnd *pitem = task_group_ends_data_.add_groups();

			pitem->set_gstate(gstate);
			pitem->set_ver_(row.getUInt(column++));

			pitem->set_iid(row.getInt64(column++));
			//pitem->set_role_iid(row.getInt64(column++));
			column++;
			pitem->set_task_group(row.getInt(column++));
			//pitem->set_group_cell(row.getInt(column++));
			column++;

			//std::string cdata = row.getString(column++);
			//pitem->mutable_cell_data()->ParseFromArray(cdata.c_str(), cdata.size());
			column++;

			//pitem->set_trigg_level(row.getInt(column++));
			column++;
			//pitem->set_createtime(row.getInt(column++));
			column++;
			pitem->set_endtime(row.getInt(column++));
		}
	}
}

void UserTasks::load_from_database2(sql::ResultSet& row)
{
	tasks_data_.Clear();
	tasks_update_ = false;

	while (row.next())
	{
		int column = 1;
		S_INT_32 qstate = row.getInt(column++);

		if (qstate < (int)PRO::TASKSTATE_SUBMIT)
		{
			PRO::DBUserTaskItem *pitem = tasks_data_.add_items();

			pitem->set_qstate(qstate);
			pitem->set_ver_(row.getUInt(column++));

			pitem->set_iid(row.getInt64(column++));
			pitem->set_role_iid(row.getInt64(column++));
			pitem->set_task_iid(row.getInt(column++));
			pitem->set_my_taskgroup(row.getInt64(column++));
			pitem->set_task_group(row.getInt(column++));
			pitem->set_accept_level(row.getInt(column++));
			pitem->set_cycle_task(row.getInt(column++) != 0);
			pitem->set_cycle_num(row.getInt(column++));
			pitem->set_createtime(row.getInt(column++));
			pitem->set_firstupdatetime(row.getInt(column++));
			pitem->set_lastupdatetime(row.getInt(column++));

			std::string tdata = row.getString(column++);
			pitem->mutable_task_datas()->ParseFromArray(tdata.c_str(), tdata.size());

			pitem->set_source_iid(row.getString(column++).c_str());
		}
		else
		{
			PRO::DBUserTaskEndItem *pitem = task_ends_data_.add_items();

			pitem->set_qstate(qstate);
			pitem->set_ver_(row.getUInt(column++));

			pitem->set_iid(row.getInt64(column++));
			//pitem->set_role_iid(row.getInt64(column++));
			column++;
			pitem->set_task_iid(row.getInt(column++));
			//pitem->set_my_taskgroup(row.getInt64(column++));
			column++;
			//pitem->set_task_group(row.getInt(column++));
			column++;
			//pitem->set_accept_level(row.getInt(column++));
			column++;
			//pitem->set_cycle_task(row.getInt(column++) != 0);
			column++;
			//pitem->set_cycle_num(row.getInt(column++));
			column++;
			//pitem->set_createtime(row.getInt(column++));
			column++;
			//pitem->set_firstupdatetime(row.getInt(column++));
			column++;
			pitem->set_lastupdatetime(row.getInt(column++));

			//std::string tdata = row.getString(column++);
			//pitem->mutable_task_datas()->ParseFromArray(tdata.c_str(), tdata.size());
			column++;

			//pitem->set_source_iid(row.getString(column++).c_str());
			column++;
		}
	}
}

void UserTasks::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS, &task_groups_data_);

	task_group_ends_data_.Clear();
	task_group_ends_data_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS_END, &task_group_ends_data_);

	//-----------------------------
	tasks_data_.Clear();
	tasks_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKS, &tasks_data_);

	task_ends_data_.Clear();
	task_ends_data_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKS_END, &task_ends_data_);
}

bool UserTasks::update_redis_groupcache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!taskgroups_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS, &task_groups_data_, svrApp.get_redisprotocache()))
	{
		taskgroups_data_update_ = false;
		return true;
	}

	return false;
}

bool UserTasks::update_redis_groupendcache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!task_group_ends_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS_END, &task_group_ends_data_, svrApp.get_redisprotocache()))
	{
		task_group_ends_data_update_ = false;
		return true;
	}

	return false;
}

bool UserTasks::update_redis_taskcache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!tasks_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKS, &tasks_data_, svrApp.get_redisprotocache()))
	{
		tasks_update_ = false;
		return true;
	}

	return false;
}

bool UserTasks::update_redis_taskendcache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!task_ends_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKS_END, &task_ends_data_, svrApp.get_redisprotocache()))
	{
		task_ends_data_update_ = false;
		return true;
	}

	return false;
}

bool UserTasks::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	update_redis_groupcache(roleid, rdv, force);
	update_redis_groupendcache(roleid, rdv, force);
	update_redis_taskcache(roleid, rdv, force);
	update_redis_taskendcache(roleid, rdv, force);

	return true;
}

PRO::DBUserTaskGroup* UserTasks::new_taskgroup()
{
	taskgroups_data_update_ = true;

	PRO::DBUserTaskGroup* tg = task_groups_data_.add_groups();
	return tg;
}

PRO::DBUserTaskGroup* UserTasks::get_taskgroup_for_update(S_INT_32 groupiid)
{
	for (int ii = 0; ii < task_groups_data_.groups_size(); ++ii)
	{
		PRO::DBUserTaskGroup* tp = task_groups_data_.mutable_groups(ii);
		if (tp->task_group() == groupiid)
		{
			taskgroups_data_update_ = true;
			return tp;
		}
	}

	return 0;
}

PRO::DBUserTaskGroupEnd* UserTasks::end_taskgroup(S_INT_32 groupid, PRO::TASK_GROUP_STATE gstate)
{
	PRO::DBUserTaskGroupEnd* pEnd = 0;

	RepeatedPtrField<PRO::DBUserTaskGroup>* groups = task_groups_data_.mutable_groups();
	for (RepeatedPtrField<PRO::DBUserTaskGroup>::const_iterator iter = groups->begin(); iter != groups->end(); )
	{
		const PRO::DBUserTaskGroup& it = (*iter);
		if (it.task_group() == groupid)
		{
			pEnd = task_group_ends_data_.add_groups();
			pEnd->set_iid(it.iid());
			pEnd->set_task_group(it.task_group());
			pEnd->set_gstate((S_INT_32)gstate);
			pEnd->set_endtime(OSSystem::mOS->GetUnixTimestamp());

			groups->erase(iter);
			break;
		}
	}

	if (pEnd)
	{
		task_group_ends_data_update_ = true;
	}

	return pEnd;
}

PRO::DBUserTaskItem* UserTasks::new_taskitem()
{
	tasks_update_ = true;
	PRO::DBUserTaskItem* pi = tasks_data_.add_items();

	return pi;
}

PRO::DBUserTaskItem* UserTasks::get_taskitem_for_update(S_INT_32 taskid)
{
	for (int ii = 0; ii < tasks_data_.items_size(); ++ii)
	{
		PRO::DBUserTaskItem* tp = tasks_data_.mutable_items(ii);
		if (tp->task_iid() == taskid)
		{
			tasks_update_ = true;
			return tp;
		}
	}

	return 0;
}

PRO::DBUserTaskItem* UserTasks::get_taskitem(S_INT_32 taskid)
{
	for (int ii = 0; ii < tasks_data_.items_size(); ++ii)
	{
		PRO::DBUserTaskItem* tp = tasks_data_.mutable_items(ii);
		if (tp->task_iid() == taskid)
		{
			return tp;
		}
	}

	return 0;
}

PRO::DBUserTaskEndItem* UserTasks::end_taskitem(S_INT_32 taskid, PRO::TASK_STATE qstate)
{
	PRO::DBUserTaskEndItem* pEnd = 0;

	RepeatedPtrField<PRO::DBUserTaskItem>* items = tasks_data_.mutable_items();
	for (RepeatedPtrField<PRO::DBUserTaskItem>::const_iterator iter = items->begin(); iter != items->end(); )
	{
		const PRO::DBUserTaskItem& it = (*iter);
		if (it.task_iid() == taskid)
		{
			pEnd = task_ends_data_.add_items();

			pEnd->set_iid(it.iid());
			pEnd->set_task_iid(it.task_iid());
			pEnd->set_qstate((S_INT_32)qstate);
			pEnd->set_lastupdatetime(OSSystem::mOS->GetUnixTimestamp());

			items->erase(iter);
			break;
		}
	}

	if (pEnd)
	{
		task_ends_data_update_ = true;
	}

	return pEnd;
}

bool UserTasks::is_taskgroup_end(S_INT_32 gid)
{
	for (int ii = 0; ii < task_group_ends_data_.groups_size(); ++ii)
	{
		const PRO::DBUserTaskGroupEnd& t = task_group_ends_data_.groups(ii);
		if (t.task_group() == gid)
			return true;
	}

	return false;
}
