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

#ifndef __DBSCTRL_H__
#define __DBSCTRL_H__

#include <boost/thread/thread.hpp>
#include <boost/thread/condition_variable.hpp>

#include <list>

#include <mysqlc++/mysql_connection.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/redis/RedisClient.h>

#include "dbs/dbThread.h"
#include "dbs/cmd/BaseDBCmd.h"

class DBSCtrl
{
private:
	DBSCtrl();
	~DBSCtrl();

public:
	static DBSCtrl* instance();
	bool init_dbsctrl();
	void uninit_dbsctrl();

	sql::Connection* get_new_connection();

	void post_db_cmd(BaseDBCmd* pcmd);
	void post_head_db_cmd(BaseDBCmd* pcmd);
	BaseDBCmd* pop_db_cmd();

	S_INT_64 gen_dbid(const char* tablename, RedisClient* rdv);

protected:
	void release();

protected:

	ThreadLock	lock_;
	std::list<BaseDBCmd*>	cmds_;

private:
	std::list<DBThread*>	thread_objs_;
	boost::thread_group     thread_pool_;
};

#define dbsStore DBSCtrl::instance()

#endif //__DBSCTRL_H__
