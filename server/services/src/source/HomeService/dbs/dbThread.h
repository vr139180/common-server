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

#ifndef __DBTHREAD_H__
#define __DBTHREAD_H__

#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <mysqlc++/mysql_connection.h>

class DBSCtrl;

class DBThread
{
public:
	DBThread(DBSCtrl* p);
	~DBThread();

	void init();
	void run();
	void finish();

protected:
	bool connection_active();

protected:
	std::shared_ptr<sql::Connection>	p_mysql_connection_;
	DBSCtrl*							parent_;
	bool								finished_;
};

#endif //__DBTHREAD_H__