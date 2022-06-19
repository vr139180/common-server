#ifndef __USERBASE_H__
#define __USERBASE_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserBase : public UserOperate
{
	friend class LobbyUser;
public:
	UserBase();

	virtual void reset_data();

	virtual bool is_modify() { return base_data_update_; }

	virtual void load_from_database(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &base_data_; }

	void data_copyall(UserBase& from);

	bool update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force = false);
	bool active_14days(S_INT_64 roleid, RedisClient* rdv);

	void load_from_redis(S_INT_64 roleid, RedisClient* rdv);

public:
	void new_rolebaseinfo(RedisClient* rdv, S_INT_64 uid, const char* nickname);

public:
	PRO::DBRoleBaseInfo& data() { return base_data_; }

protected:
	PRO::DBRoleBaseInfo	base_data_;
	bool				base_data_update_;
};

#endif //__USERBASE_H__
