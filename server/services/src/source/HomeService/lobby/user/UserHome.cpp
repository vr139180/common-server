#include "lobby/user/UserHome.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

UserHome::UserHome():home_data_update_(false)
{

}

void UserHome::reset_data()
{
	home_data_update_ = false;
}

void UserHome::data_copyall(UserHome& from)
{
	data_copy_from<PRO::DBUserHome>(&from.home_data_);
}

void UserHome::load_from_database(sql::ResultSet& row)
{
	home_data_update_ = false;

	while (row.next())
	{
		int column = 1;
		home_data_.set_ver_(row.getUInt(column++));

		home_data_.set_role_iid(row.getInt64(column++));
		home_data_.set_home_name(row.getString(column++).c_str());
		home_data_.set_ground_resid(row.getInt64(column++));
		home_data_.set_look_at(row.getString(column++).c_str());
		home_data_.set_geo_pos(row.getString(column++).c_str());
		home_data_.set_reside_time(row.getInt(column++));
		home_data_.set_last_residedate(row.getInt(column++));
		home_data_.set_levels(row.getInt(column++));
	}
}

void UserHome::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	home_data_update_ = false;

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);
	rdv->get_hashobject(key.c_str(), USER_DETAIL_USERHOME, &home_data_);
}

bool UserHome::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!home_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_USERHOME, &home_data_, svrApp.get_redisprotocache()))
	{
		home_data_update_ = false;
		return true;
	}
	return false;
}

void UserHome::new_userhome(S_UINT_32 ver, S_INT_64 roleid, const char* homename, RedisClient* rdv)
{
	home_data_.set_ver_(ver);

	home_data_.set_role_iid(roleid);
	home_data_.set_home_name(homename);
	
	home_data_.set_ground_resid(1);
	home_data_.set_look_at("{0,0,0}");
	home_data_.set_geo_pos("{0,0,0}");
	home_data_.set_reside_time(1);
	home_data_.set_last_residedate(OSSystem::mOS->GetUnixTimestamp());
	home_data_.set_levels(1);

	home_data_update_ = true;
}

S_INT_64 UserHome::get_home_iid()
{
	return home_data_.role_iid();
}
