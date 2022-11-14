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

#ifndef __USERBUILDINGS_H__
#define __USERBUILDINGS_H__

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserOperate.h"

class LobbyUser;

class UserBuildings : public UserOperate
{
	friend class LobbyUser;
public:
	UserBuildings();

	virtual void reset_data();

	virtual void load_from_database(sql::ResultSet& row);

	virtual google::protobuf::Message* get_data() { return &building_data_; }
	virtual google::protobuf::Message* get_data_dels() { return &dels_; }

	virtual bool is_modify() { return building_data_update_ || dels_update_; }
	virtual void mark_data_dels_updated() { dels_update_ = true; }

	void data_copyall(UserBuildings& from);

	bool update_redis_datacache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_delscache(S_INT_64 uid, RedisClient* rdv, bool force = false);
	bool update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force = false);

	void load_from_redis(S_INT_64 roleid, RedisClient* rdv);

public:
	PRO::DBUserHomeStructureItem* add_item();

	bool del_item(S_INT_64 iid, S_UINT_32 ver);

protected:
	PRO::DBUserHomeStructure	building_data_;
	bool						building_data_update_;
	PRO::DBRowDeletes	dels_;
	bool				dels_update_;
};

#endif //__USERBUILDINGS_H__
