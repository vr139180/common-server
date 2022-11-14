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

#ifndef __USERHOME_H__
#define __USERHOME_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserHome : public UserOperate
{
	friend class LobbyUser;
public:
	UserHome();

	virtual void reset_data();

	virtual bool is_modify() { return home_data_update_; }

	virtual void load_from_database(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &home_data_; }

	void data_copyall(UserHome& from);

	bool update_redis_cache(S_INT_64 uid, RedisClient* rdv, bool force = false);

	void load_from_redis(S_INT_64 roleid, RedisClient* rdv);

	S_INT_64 get_home_iid();

public:
	void new_userhome( S_UINT_32 ver, S_INT_64 roleid, const char* homename, RedisClient* rdv);

public:
	PRO::DBUserHome& data() { return home_data_; }

protected:
	PRO::DBUserHome	home_data_;
	bool			home_data_update_;
};

#endif //__USERHOME_H__
