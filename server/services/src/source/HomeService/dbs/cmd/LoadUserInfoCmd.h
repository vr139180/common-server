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

#ifndef __LOADUSERINFOCMD_H__
#define __LOADUSERINFOCMD_H__

#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserBuildings.h"
#include "lobby/user/UserPets.h"
#include "lobby/user/UserBase.h"
#include "lobby/user/UserHome.h"
#include "lobby/user/UserTasks.h"

#include "dbs/cmd/BaseDBCmd.h"

class LoadUserInfoCmd : public BaseDBCmd
{
public:
	LoadUserInfoCmd(S_INT_64 rid, S_INT_64 uid, S_INT_64 token, LobbyService* p);

	void reuse_cmd(S_INT_64 uid, S_INT_64 token);

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection);
	//run in logic thread
	virtual void run();

	virtual bool reused() { return true; }

protected:
	UserBase		base_data_;
	UserHome		home_data_;
	UserBuildings	building_data_;
	UserPets		pet_data_;
	UserTasks		task_data_;

	S_INT_64		role_iid_;

	bool success_;
};

#endif //__LOADUSERINFOCMD_H__
