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

UserLoginCmd::UserLoginCmd(const char* uname, const char* pwd, bool bloadroles, StateService* p):BaseDBCmd( p)
, result_(0)
, user_iid_(0)
, b_need_load_roles_(bloadroles)
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
		result_ = 0;

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

		if ((result_ == 0 || result_ == 1)&& b_need_load_roles_)
		{
			release_dboperator(prep_stmt.get());

			prep_stmt.reset(p_connection->prepareStatement(
				"select ver_,role_iid,user_iid,nickname,unix_timestamp(registime),levels from role_baseinfo where user_iid = ?;"));
			prep_stmt->setInt64(1, user_iid_);
			res.reset(prep_stmt->executeQuery());
			while (res->next())
			{
				sql::ResultSet* rss = res.get();
				PRO::DBRoleBaseInfo *prinfo = roles_data_.add_roles();

				int column = 1;
				prinfo->set_ver_(rss->getUInt(column++));

				prinfo->set_role_iid(rss->getInt64(column++));
				prinfo->set_user_iid(rss->getInt64(column++));
				prinfo->set_nickname(rss->getString(column++).c_str());
				prinfo->set_registime(rss->getInt(column++));
				prinfo->set_levels(rss->getInt(column++));
			}
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
	head_.set_token_userid(user_iid_);
	state_->on_db_user_login_act(head_, result_, 1, username_.c_str(), b_need_load_roles_, roles_data_);
}
