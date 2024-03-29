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

#ifndef __LOADUSERROLESCMD_H__
#define __LOADUSERROLESCMD_H__

#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserRoles.h"

#include "dbs/cmd/BaseDBCmd.h"

class LoadUserRolesCmd : public BaseDBCmd
{
public:
	LoadUserRolesCmd(const SProtocolHead& head, LobbyService* p);

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection);
	//run in logic thread
	virtual void run();

	virtual bool reused() { return true; }

protected:
	UserRoles	roles_data_;

	bool		success_;
};

#endif //__LOADUSERROLESCMD_H__
