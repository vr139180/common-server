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
, result_(0)
, user_iid_(0)
{
	username_ = uname;
	pwd_ = pwd;
}

void UserLoginCmd::reuse_cmd(const SProtocolHead& h)
{
	this->head_ = h;
}

void UserLoginCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::PreparedStatement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->prepareStatement(
			"select user_iid, pwd, state from user_account where account = ?;"));

		prep_stmt->setString(1, username_.c_str());

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());

		if (res->next())
		{
			user_iid_ = res->getInt64(1);
			user_pwd_ = res->getString(2).c_str();
			user_state_ = res->getInt(3);

			if (user_state_ != 0)
				result_ = 1;
			else if (user_pwd_ != pwd_)
				result_ = 3;
		}
		else
		{
			result_ = 2;
		}
	}
	catch (sql::SQLException& err)
	{
		logError(out_runtime, "database UserLoginCmd error:%s", err.what());
		result_ = 4;
	}

	release_dboperator(prep_stmt.get());
}

void UserLoginCmd::run()
{
	//在state线程回调
	state_->on_db_user_login_act(head_, result_, 1, username_.c_str(), user_iid_);
}
