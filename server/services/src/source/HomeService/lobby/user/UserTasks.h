#ifndef __USERTASKS_H__
#define __USERTASKS_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserTasks : public UserOperate
{
	friend class LobbyUser;
public:
	UserTasks();

	virtual void reset_data();

	virtual void load_from_database1(sql::ResultSet& row);
	virtual void load_from_database2(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &task_groups_data_; }

	void data_copyall(UserTasks& from);

	bool update_redis_datacache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force = false);

	void load_from_redis(S_INT_64 roleid, RedisClient* rdv);

	virtual bool is_modify() { return taskgroups_data_update_ || tasks_update_; }

public:
	PRO::DBUserTaskGroup* new_taskgroup();
	const PRO::DBUserTaskGroup* get_taskgroup_for_update(S_INT_32 groupiid);

	PRO::DBUserTaskItem* new_taskitem();

protected:
	PRO::DBUserTaskGroups	task_groups_data_;
	bool					taskgroups_data_update_;
	PRO::DBUserTasks		tasks_data_;
	bool					tasks_update_;
};

#endif //__USERTASKS_H__
