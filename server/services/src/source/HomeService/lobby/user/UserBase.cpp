#include "lobby/user/UserBase.h"

#include <cmsLib/base/OSSystem.h>
#include <gameLib/redis/user_redis_const.h>

#include "dbs/DBSCtrl.h"
#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

UserBase::UserBase():base_data_update_(false)
{

}

void UserBase::reset_data()
{
	base_data_update_ = false;
}

void UserBase::data_copyall(UserBase& from)
{
	data_copy_from<PRO::DBRoleBaseInfo>(&from.base_data_);
}

void UserBase::load_from_database(sql::ResultSet& row)
{
	base_data_update_ = false;

	while (row.next())
	{
		int column = 1;
		base_data_.set_ver_(row.getUInt(column++));

		base_data_.set_role_iid(row.getInt64(column++));
		base_data_.set_user_iid(row.getInt64(column++));
		base_data_.set_nickname(row.getString(column++).c_str());
		base_data_.set_registime(row.getInt(column++));
	}
}

void UserBase::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	base_data_update_ = false;

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);
	rdv->get_hashobject(key.c_str(), USER_DETAIL_USERBASE, &base_data_);
}

bool UserBase::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!base_data_update_)
			return false;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_USERBASE, &base_data_, svrApp.get_redisprotocache()))
	{
		base_data_update_ = true;
		return true;
	}

	return false;
}

bool UserBase::active_14days(S_INT_64 roleid, RedisClient* rdv)
{
	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);
	return rdv->pexpire(key.c_str(), REDIS_USER_LIFETIME);
}

void UserBase::new_rolebaseinfo(RedisClient* rdv, S_INT_64 uid, const char* nickname)
{
	S_INT_64 rid = dbsStore->gen_dbid( DBID_ROLE_BASEINFO, rdv);
	
	base_data_.set_ver_(DBVER_INIT);

	base_data_.set_role_iid(rid);
	base_data_.set_user_iid(uid);
	base_data_.set_nickname(nickname);
	base_data_.set_registime(OSSystem::mOS->GetUnixTimestamp());

	base_data_update_ = true;
}
