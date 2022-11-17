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

#ifndef __LOGSAVEUTIL_H__
#define __LOGSAVEUTIL_H__

#include <cmsLib/core_type.h>
#include <cmsLib/ThreadLock.h>
#include <boost/thread/condition_variable.hpp>
#include <mysqlc++/mysql_connection.h>

typedef struct tagLogData {
	S_INT_64 timestamp;
	std::string openid;
	std::string accountid;
	std::string action;
	std::string protoid;
	S_INT_32 delay;
	S_INT_32 result;
}LogData;

class LogSaveUtil
{
public:
	LogSaveUtil();
	~LogSaveUtil();

	void start_save();
	void stop_save();

	void save_log(const char* strlog);

	bool is_saveing() { return dwThreadId > 0; }
	int get_lognum();
	void set_no_logscaned() { no_log_scaned_ = true; }

protected:
	static DWORD  WINAPI Thread(LPVOID  lparam);
	void main_loop();

	void set_cancel();

	void release();

	void do_save(sql::Connection* p_connection, std::list<LogData*>& pDatas);

protected:
	std::list<LogData*> logs_;
	ThreadLock lock_;
	volatile int saved_nums_;

	bool no_log_scaned_;

public:
	//用户名口令
	std::string dbuser_;
	std::string dbpwd_;
	//数据库名字
	std::string dbname_;
	std::string dbip_;

private:
	bool	cancel_;
	HANDLE	hthread;
	DWORD	dwThreadId;
};

#endif //__UPLOADLOGUTIL_H__