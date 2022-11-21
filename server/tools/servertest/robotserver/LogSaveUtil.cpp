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

#include "StdAfx.h"

#include "LogSaveUtil.h"

#include <cppconn/exception.h>
#include <cppconn/metadata.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

#include <mysqlc++/mysql_driver.h>

#include "OptionUtil.h"

extern OptionUtil g_opt;

LogSaveUtil::LogSaveUtil()
{
}

LogSaveUtil::~LogSaveUtil()
{
}

void LogSaveUtil::start_save()
{
	std::shared_ptr<sql::Connection>	p_mysql_connection_;
	sql::Connection* conn = 0;
	try
	{
		sql::Driver* p_driver = sql::mysql::get_driver_instance();

		sql::ConnectOptionsMap cmap;
		cmap["hostName"] = g_opt.db_.dbip_;
		cmap["userName"] = g_opt.db_.dbuser_;
		cmap["password"] = g_opt.db_.dbpwd_;
		cmap["schema"] = g_opt.db_.dbname_;
		cmap["OPT_RECONNECT"] = true;

		conn = p_driver->connect(cmap);
		if (conn == 0)
			return;
		p_mysql_connection_.reset(conn);
	}
	catch (const sql::SQLException& err)
	{
		return;
	}

	sql::mysql::get_driver_instance()->threadInit();

	clear_logs(conn);

	try {
		p_mysql_connection_->close();
	}
	catch (...) {}

	sql::mysql::get_driver_instance()->threadEnd();
}

void LogSaveUtil::clear_logs(sql::Connection* p_connection)
{
	std::unique_ptr< sql::PreparedStatement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->prepareStatement("delete from test_logs;"));

		prep_stmt->execute();
	}
	catch (sql::SQLException& err)
	{
	}
}
