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

#include "dbs/cmd/UserLoginCmd.h"

#include <gameLib/LogExt.h>

#include "StateServiceApp.h"

UserLoginCmd::UserLoginCmd(const char* uname, const char* pwd, StateService* p):BaseDBCmd( p)
, success_(false)
, user_iid_(0)
, user_token_(0)
{
	username_ = uname;
	pwd_ = pwd;
}

void UserLoginCmd::reuse_cmd(S_INT_64 uid, S_INT_64 token)
{
	this->user_iid_ = uid;
	this->user_token_ = token;
}

void UserLoginCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::PreparedStatement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->prepareStatement(
			"select user_iid, pwd, state from user_account where account=?);"));

		prep_stmt->setString(1, username_.c_str());

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		if (res->next())
		{
			success_ = true;
			user_iid_ = res->getInt64(1);
			user_pwd_ = res->getString(2).c_str();
			user_state_ = res->getInt(3);
		}
	}
	catch (sql::SQLException& err)
	{
		logError(out_runtime, "database UserLoginCmd error:%s", err.what());
	}

	release_dboperator(prep_stmt.get());
}

void UserLoginCmd::run()
{
	if (!success_) return;

}
