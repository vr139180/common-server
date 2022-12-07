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
		base_data_.set_levels(row.getInt(column++));

		PRO::Location3D* pos = base_data_.mutable_loc();
		pos->set_x((float)row.getDouble(column++));
		pos->set_y((float)row.getDouble(column++));
		pos->set_z((float)row.getDouble(column++));
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
	base_data_.set_levels(1);
 	PRO::Location3D* pos = base_data_.mutable_loc();
	GLoc3D::copy_to(GLoc3D::zero_point(), pos);

	base_data_update_ = true;
}

void UserBase::update_role_loc(S_UINT_32 ver, const GLoc3D& loc)
{
	base_data_.set_ver_(ver);

	PRO::Location3D* pos = base_data_.mutable_loc();
	GLoc3D::copy_to(loc, pos);

	base_data_update_ = true;
}
