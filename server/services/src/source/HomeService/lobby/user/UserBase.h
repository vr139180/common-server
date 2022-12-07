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

#ifndef __USERBASE_H__
#define __USERBASE_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>
#include <gameLib/commons/GLoc3D.h>

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

	void update_role_loc(S_UINT_32 ver, const GLoc3D& loc);

	S_INT_32 get_levels() { return base_data_.levels(); }
	const char* get_nickname() const { return base_data_.nickname().c_str(); }

public:
	PRO::DBRoleBaseInfo& data() { return base_data_; }

protected:
	PRO::DBRoleBaseInfo	base_data_;
	bool				base_data_update_;
};

#endif //__USERBASE_H__
