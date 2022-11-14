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
