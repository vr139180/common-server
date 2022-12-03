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

		PRO::Location3D* pos = prinfo->mutable_loc();
		pos->set_x((float)row.getDouble(column++));
		pos->set_y((float)row.getDouble(column++));
		pos->set_z((float)row.getDouble(column++));
	}
}

bool UserRoles::load_from_redis(S_INT_64 uid, RedisClient* rdv)
{
	roles_data_.Clear();
	roles_data_update_ = false;

	std::string key = rdv->build_rediskey(USER_USERINFO, uid);
	return rdv->get_hashobject(key.c_str(), USER_UINFO_F_ROLES, &roles_data_);
}

bool UserRoles::update_redis_cache(S_INT_64 uid, RedisClient* rdv)
{
	roles_data_update_ = false;
	std::string key = rdv->build_rediskey(USER_USERINFO, uid);
	
	if( rdv->set_hashobject( key.c_str(), USER_UINFO_F_ROLES, &roles_data_, svrApp.get_redisprotocache()))
	{
		roles_data_update_ = false;
		return true;
	}
	return true;
}

void UserRoles::role_update_loc(S_INT_64 uid, S_INT_64 rid, const GLoc3D& loc)
{
	PRO::DBRoleBaseInfo* prole = 0;
	for (int ii = 0; ii < roles_data_.roles_size(); ++ii)
	{
		const PRO::DBRoleBaseInfo& r = roles_data_.roles(ii);
		if (r.role_iid() == rid)
		{
			prole = (PRO::DBRoleBaseInfo*)&r;
			break;
		}
	}

	if (prole != 0)
	{
		PRO::Location3D* pos = prole->mutable_loc();
		GLoc3D::copy_to(loc, pos);

		RedisClient* rdv = svrApp.get_redisclient();
		update_redis_cache(uid, rdv);
	}
}
