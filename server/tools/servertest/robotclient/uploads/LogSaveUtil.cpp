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

#include "uploads/LogSaveUtil.h"

#include <cppconn/exception.h>
#include <cppconn/metadata.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

#include <mysqlc++/mysql_driver.h>

extern CWindow	*g_mainwnd;

LogSaveUtil::LogSaveUtil() :
	cancel_(false),
	hthread(0),
	dwThreadId(0)
{
}

LogSaveUtil::~LogSaveUtil()
{
	stop_save();
}

void LogSaveUtil::release()
{
	ThreadLockWrapper wrapper(lock_);

	for (std::list<LogData*>::iterator iter = logs_.begin(); iter != logs_.end(); ++iter)
	{
		delete (*iter);
	}
	logs_.clear();
}

void LogSaveUtil::start_save()
{
	if (hthread != 0)
		return;

	cancel_ = false;
	hthread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, this, 0, &dwThreadId);
}

void LogSaveUtil::stop_save()
{
	release();

	cancel_ = true;
	if (hthread > 0)
	{
		::WaitForSingleObject(hthread, INFINITE);
		CloseHandle(hthread);
	}

	hthread = 0;
	dwThreadId = 0;
}

DWORD  WINAPI LogSaveUtil::Thread(LPVOID  lparam)
{
	LogSaveUtil *pThis = reinterpret_cast<LogSaveUtil*>(lparam);

	pThis->main_loop();

	return 1;
}

void LogSaveUtil::set_cancel()
{
	cancel_ = true;
};

void LogSaveUtil::main_loop()
{
	no_log_scaned_ = false;

	saved_nums_ = 0;

	sql::ConnectOptionsMap cmap;
	cmap["hostName"] = dbip_;
	cmap["userName"] = dbuser_;
	cmap["password"] = dbpwd_;
	cmap["schema"] = dbname_;
	cmap["OPT_RECONNECT"] = true;

	std::shared_ptr<sql::Connection>	p_mysql_connection_;
	sql::Connection* conn = 0;

	sql::Driver* p_driver = sql::mysql::get_driver_instance();
	conn = p_driver->connect(cmap);
	p_mysql_connection_.reset(conn);

	sql::mysql::get_driver_instance()->threadInit();

	int insert_num = 0;

	while (1)
	{
		if (cancel_)
			break;
		::Sleep(100);

		std::list<LogData*> xlogs;
		{
			ThreadLockWrapper wrapper(lock_);

			if (logs_.size() == 0 && no_log_scaned_)
				break;

			int cnt = 0;

			std::list<LogData*>::iterator iter = logs_.begin();
			while( iter != logs_.end() && cnt <= 50)
			{
				++cnt;
				xlogs.push_back(*iter);

				logs_.erase(iter);
				iter = logs_.begin();
			}
			
			if( xlogs.size() == 0)
				continue;
		}

		do_save(conn, xlogs);

		for (std::list<LogData*>::iterator iter = xlogs.begin(); iter != xlogs.end(); ++iter)
		{
			delete (*iter);

			insert_num++;
		}

		if (insert_num > 5000)
		{
			insert_num = 0;
			conn->close();
			::Sleep(2000);

			sql::Driver* p_driver = sql::mysql::get_driver_instance();
			conn = p_driver->connect(cmap);
			p_mysql_connection_.reset(conn);
		}
	}

	try {
		p_mysql_connection_->close();
	}
	catch (...) {}

	sql::mysql::get_driver_instance()->threadEnd();

	g_mainwnd->PostMessage(WM_USER + 0x202, (WPARAM)0, 0);
}

void LogSaveUtil::do_save(sql::Connection* p_connection, std::list<LogData*>& xlogs)
{
	std::unique_ptr< sql::PreparedStatement > prep_stmt;
	try
	{
		int column = 0;
		//------------------------------------------------------------------------------------------------
		prep_stmt.reset(p_connection->prepareStatement(
			"insert into test_logs(timestamp,openid,accountid,action,protoid,delay,result) values(?,?,?,?,?,?,?);"));

		for (std::list<LogData*>::iterator iter = xlogs.begin(); iter != xlogs.end(); ++iter)
		{
			LogData* pData = (*iter);

			column = 1;
			prep_stmt->setInt64(column++, pData->timestamp);
			prep_stmt->setString(column++, pData->openid.c_str());
			prep_stmt->setInt64(column++, pData->accountid);
			prep_stmt->setString(column++, pData->action.c_str());
			prep_stmt->setString(column++, pData->protoid.c_str());
			prep_stmt->setInt(column++, pData->delay);
			prep_stmt->setInt(column++, pData->result);

			prep_stmt->execute();

			++saved_nums_;
		}
	}
	catch (sql::SQLException& err)
	{
	}
}

#include <cmsLib/json/JSONUtil.h>

void LogSaveUtil::save_log(const char* strlog)
{
	if (strlog == 0)
		return;

	try {
		boost::json::parse_options opt;
		opt.allow_comments = true;
		opt.allow_trailing_commas = true;

		boost::json::error_code ec;
		boost::json::value root = boost::json::parse(strlog, ec, {}, opt);
		if (ec.failed())
			return;

		LogData* ret = new LogData();
		std::unique_ptr<LogData> ptr(ret);

		boost::json::object& obj = root.as_object();
		ret->timestamp = JSONUtil::get_int64(obj, "timestamp", 0);
		ret->openid = JSONUtil::get_string(obj, "openid");
		ret->accountid = JSONUtil::get_int64(obj, "accountid");
		ret->action = JSONUtil::get_string(obj, "action");
		ret->protoid = JSONUtil::get_string(obj, "protoid","0");
		ret->delay = JSONUtil::get_value<int>(obj, "delay", 0);
		ret->result = JSONUtil::get_value<int>(obj, "result", 0);

		{
			ThreadLockWrapper wrapper(lock_);
			logs_.push_back(ptr.release());
		}
	}
	catch (...) {
	}
}

int LogSaveUtil::get_lognum()
{
	return saved_nums_;
}
