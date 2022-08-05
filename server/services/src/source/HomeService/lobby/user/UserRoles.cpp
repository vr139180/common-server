#include "lobby/user/UserRoles.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

UserRoles::UserRoles():roles_data_update_(false)
{

}

void UserRoles::reset_data()
{
	roles_data_update_ = false;
	roles_data_.Clear();
}

bool UserRoles::is_role_exist(S_INT_64 rid)
{
	for (int ii = 0; ii < roles_data_.roles_size(); ++ii)
	{
		const PRO::DBRoleBaseInfo& r = roles_data_.roles(ii);
		if (r.role_iid() == rid)
			return true;
	}

	return false;
}

void UserRoles::data_copyall(UserRoles& from)
{
	data_copy_from<PRO::DBUserRoles>(&from.roles_data_);
}

void UserRoles::load_from_database(sql::ResultSet& row)
{
	roles_data_.Clear();
	roles_data_update_ = false;

	while (row.next())
	{
		PRO::DBRoleBaseInfo *prinfo = roles_data_.add_roles();

		int column = 1;
		prinfo->set_ver_(row.getUInt(column++));

		prinfo->set_role_iid(row.getInt64(column++));
		prinfo->set_user_iid(row.getInt64(column++));
		prinfo->set_nickname(row.getString(column++).c_str());
		prinfo->set_registime(row.getInt(column++));
		prinfo->set_levels(row.getInt(column++));
	}
}

bool UserRoles::load_from_redis(S_INT_64 uid, RedisClient* rdv)
{
	roles_data_.Clear();
	roles_data_update_ = false;

	std::string key = rdv->build_rediskey(USER_ROLES, uid);
	return rdv->get(key.c_str(), &roles_data_, REDIS_USER_LIFETIME);
}

bool UserRoles::update_redis_cache(S_INT_64 uid, RedisClient* rdv)
{
	roles_data_update_ = false;
	std::string key = rdv->build_rediskey(USER_ROLES, uid);
	
	if (rdv->add(key.c_str(), &roles_data_, svrApp.get_redisprotocache(), REDIS_USER_LIFETIME))
	{
		roles_data_update_ = false;
		return true;
	}
	return true;
}
