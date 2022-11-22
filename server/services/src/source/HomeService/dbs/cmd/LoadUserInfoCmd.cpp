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

#include "dbs/cmd/LoadUserInfoCmd.h"

#include <gameLib/LogExt.h>
#include "lobby/LobbyUser.h"

#include "HomeServiceApp.h"

LoadUserInfoCmd::LoadUserInfoCmd(const SProtocolHead& head, LobbyService* p):BaseDBCmd(head,p)
, success_(false)
{
	role_iid_ = head_.get_role_iid();
}

void LoadUserInfoCmd::reuse_cmd(const SProtocolHead& head)
{
	this->head_ = head;
}

void LoadUserInfoCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::Statement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->createStatement());
		std::stringstream sql;
		sql << "CALL get_role_data(" << role_iid_ << ");";

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery(sql.str().c_str()));
		//role baseinfo
		if (res.get() != 0)
		{
			base_data_.load_from_database(*res.get());
		}

		//home info
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			home_data_.load_from_database(*res.get());
		}

		//building
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			building_data_.load_from_database(*res.get());
		}

		//pets
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			pet_data_.load_from_database(*res.get());
		}

		//tasks
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			task_data_.load_from_database1(*res.get());
		}

		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			task_data_.load_from_database2(*res.get());
		}

		success_ = true;
	}
	catch (sql::SQLException& err)
	{
		logError(out_runtime, "database LoadUserInfoCmd -> call get_role_data produce error:%s", err.what());
	}

	release_dboperator(prep_stmt.get());
}

void LoadUserInfoCmd::run()
{
	if (!success_) return;

	LobbyUser* puser = lobby_->get_userbyid_from_msg(head_);
	if (puser == 0) return;

	puser->on_db_roledata_sync1( base_data_, home_data_, building_data_, pet_data_, task_data_);
	puser->role_selected_done();
}
