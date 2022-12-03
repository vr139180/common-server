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

#include "dbs/cmd/LoadUserRolesCmd.h"

#include <gameLib/LogExt.h>
#include "lobby/LobbyUser.h"

#include "HomeServiceApp.h"

LoadUserRolesCmd::LoadUserRolesCmd(const SProtocolHead& head, LobbyService* p) :BaseDBCmd( head, p)
, success_(false)
{

}

void LoadUserRolesCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::Statement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->createStatement());

		std::stringstream sql;
		sql << "select ver_,role_iid,user_iid,nickname,unix_timestamp(registime),levels,location_x,location_y,location_z from role_baseinfo where user_iid = "
			<< head_.get_token_useriid() << ";";

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery(sql.str().c_str()));
		//role list
		if (res.get() != 0)
		{
			sql::ResultSet* rss = res.get();
			roles_data_.load_from_database(*rss);

			success_ = true;
		}
	}
	catch (sql::SQLException& err)
	{
		logError(out_runtime, "database LoadUserRolesCmd -> call select from role_baseinfo produce error:%s", err.what());
	}

	release_dboperator(prep_stmt.get());
}

void LoadUserRolesCmd::run()
{
	if (!success_)
		return;

	LobbyUser* puser = lobby_->get_userbyid_from_msg( head_);
	if (puser == 0) return;

	puser->on_db_rolelist_update( roles_data_);
}
