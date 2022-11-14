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

#ifndef __BASEDBCMD_H__
#define __BASEDBCMD_H__

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

#include <mysqlc++/mysql_driver.h>
#include <mysqlc++/mysql_connection.h>

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/core_type.h>

class LobbyService;

class BaseDBCmd : public CommandBase
{
public:
	BaseDBCmd( S_INT_64 uid, S_INT_64 token, LobbyService* p);
	virtual ~BaseDBCmd() {}

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection) = 0;
	//run in logic thread
	virtual void run() = 0;

	//will run in db thread and logic thread
	virtual bool reused() { return false; }

	virtual void dispath_again();

protected:
	void release_dboperator(sql::Statement* ps) {
		if (ps == 0)
			return;

		try {
			std::unique_ptr<sql::ResultSet> rs;

			while (ps->getMoreResults()) {
				rs.reset(ps->getResultSet());
			}
		}
		catch (...) {}
	}

protected:
	LobbyService*	lobby_;

	S_INT_64 user_iid_;
	S_INT_64 protoken_;
};

#endif //__BASEDBCMD_H__
