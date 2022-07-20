#include "lobby/user/UserTasks.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

using namespace google::protobuf;

USED_REDISKEY_USER_NS

UserTasks::UserTasks():taskgroups_data_update_(false)
, tasks_update_(false)
{
}

void UserTasks::reset_data()
{
	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	tasks_data_.Clear();
	tasks_update_ = false;
}

void UserTasks::data_copyall(UserTasks& from)
{
	data_copy_from<PRO::DBUserTaskGroups>(&from.task_groups_data_, &task_groups_data_);
	data_copy_from<PRO::DBUserTasks>(&from.tasks_data_, &tasks_data_);
}

void UserTasks::load_from_database1(sql::ResultSet& row)
{
	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	while (row.next())
	{
		PRO::DBUserTaskGroup *pitem = task_groups_data_.add_groups();

		int column = 1;
		pitem->set_ver_(row.getUInt(column++));

		pitem->set_iid(row.getInt64(column++));
		pitem->set_role_iid(row.getInt64(column++));
		pitem->set_task_group(row.getInt(column++));
		pitem->set_group_cell(row.getInt(column++));

		std::string cdata = row.getString(column++);
		pitem->mutable_cell_data()->ParseFromArray(cdata.c_str(), cdata.size());
		
		pitem->set_gstate(row.getInt(column++));
		pitem->set_trigg_level(row.getInt(column++));
		pitem->set_createtime(row.getInt(column++));
		pitem->set_endtime(row.getInt(column++));
	}
}

void UserTasks::load_from_database2(sql::ResultSet& row)
{
	tasks_data_.Clear();
	tasks_update_ = false;

	while (row.next())
	{
		PRO::DBUserTaskItem *pitem = tasks_data_.add_items();

		int column = 1;
		pitem->set_ver_(row.getUInt(column++));

		pitem->set_iid(row.getInt64(column++));
		pitem->set_role_iid(row.getInt64(column++));
		pitem->set_task_iid(row.getInt(column++));
		pitem->set_my_taskgroup(row.getInt64(column++));
		pitem->set_task_group(row.getInt(column++));
		pitem->set_qstate(row.getInt(column++));
		pitem->set_accept_level(row.getInt(column++));
		pitem->set_cycle_task(row.getInt(column++)!=0);
		pitem->set_cycle_num(row.getInt(column++));
		pitem->set_createtime(row.getInt(column++));
		pitem->set_firstupdatetime(row.getInt(column++));
		pitem->set_lastupdatetime(row.getInt(column++));
		
		std::string tdata = row.getString(column++);
		pitem->mutable_task_datas()->ParseFromArray(tdata.c_str(), tdata.size());
		
		pitem->set_source_iid(row.getString(column++).c_str());
	}
}

void UserTasks::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	task_groups_data_.Clear();
	taskgroups_data_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS, &task_groups_data_);

	tasks_data_.Clear();
	tasks_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_TASKS, &tasks_data_);
}

bool UserTasks::update_redis_datacache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!(taskgroups_data_update_ || tasks_update_))
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (taskgroups_data_update_)
	{
		if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKGROUPS, &task_groups_data_, svrApp.get_redisprotocache()))
		{
			taskgroups_data_update_ = false;
		}
	}
	if (tasks_update_)
	{
		if (rdv->set_hashobject(key.c_str(), USER_DETAIL_TASKS, &tasks_data_, svrApp.get_redisprotocache()))
		{
			tasks_update_ = false;
		}
	}

	return true;
}

bool UserTasks::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	update_redis_datacache(roleid, rdv, force);

	return true;
}

PRO::DBUserTaskGroup* UserTasks::new_taskgroup()
{
	taskgroups_data_update_ = true;

	PRO::DBUserTaskGroup* tg = task_groups_data_.add_groups();
	return tg;
}

const PRO::DBUserTaskGroup* UserTasks::get_taskgroup_for_update(S_INT_32 groupiid)
{
	for (int ii = 0; ii < task_groups_data_.groups_size(); ++ii)
	{
		const PRO::DBUserTaskGroup& tp = task_groups_data_.groups(ii);
		if (tp.task_group() == groupiid)
		{
			taskgroups_data_update_ = true;
			return &tp;
		}
	}

	return 0;
}

PRO::DBUserTaskItem* UserTasks::new_taskitem()
{
	tasks_update_ = true;
	PRO::DBUserTaskItem* pi = tasks_data_.add_items();

	return pi;
}
