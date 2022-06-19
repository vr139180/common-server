#ifndef __USERROLES_H__
#define __USERROLES_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserRoles : public UserOperate
{
	friend class LobbyUser;
public:
	UserRoles();

	virtual void reset_data();

	virtual void load_from_database(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &roles_data_;}

	void data_copyall(UserRoles& from);

	bool update_redis_cache(S_INT_64 uid, RedisClient* rdv);
	bool load_from_redis(S_INT_64 uid, RedisClient* rdv);

	bool is_role_exist(S_INT_64 rid);

protected:
	PRO::DBUserRoles	roles_data_;
	bool				roles_data_update_;
};

#endif //__USERROLES_H__
