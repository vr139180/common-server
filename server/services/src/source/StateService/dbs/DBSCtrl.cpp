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

#include "dbs/DBSCtrl.h"

#include <mysqlc++/mysql_driver.h>
#include <cppconn/metadata.h>

#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/LogExt.h>

#include "config/StateConfig.h"
#include "StateServiceApp.h"

DBSCtrl* DBSCtrl::instance()
{
	static DBSCtrl s_instance;
	return &s_instance;
}

DBSCtrl::DBSCtrl()
{
}

DBSCtrl::~DBSCtrl()
{
}

bool DBSCtrl::init_dbsctrl()
{
	StateConfig* cf = svrApp.get_config();
	for (s32 i = 0; i < cf->db_thread_nums_; i++)
	{
		DBThread* obj_ptr = new DBThread(this);
		obj_ptr->init();

		thread_pool_.add_thread(new boost::thread(boost::bind(&DBThread::run, obj_ptr)));
		thread_objs_.push_back(obj_ptr);
	}

	return true;
}

void DBSCtrl::uninit_dbsctrl()
{
	for (std::list<DBThread*>::iterator iter = thread_objs_.begin(); iter != thread_objs_.end(); iter++)
	{
		DBThread* pThread = *iter;
		pThread->finish();
	}

	thread_pool_.join_all();

	this->release();
}

sql::Connection* DBSCtrl::get_new_connection()
{
	StateConfig* cf = svrApp.get_config();

	sql::Connection* conn = 0;
	try
	{
		sql::Driver* p_driver = sql::mysql::get_driver_instance();

		sql::ConnectOptionsMap cmap;
		cmap["hostName"] = cf->db_.dbip_;
		cmap["userName"] = cf->db_.dbuser_;
		cmap["password"] = cf->db_.dbpwd_;
		cmap["schema"] = cf->db_.dbname_;
		cmap["OPT_RECONNECT"] = true;

		conn = p_driver->connect(cmap);
	}
	catch (const sql::SQLException& err)
	{
		logError(out_runtime, "mysql database error:%s", err.what());
		return 0;
	}

	return conn;
}

void DBSCtrl::release()
{
	for (std::list<BaseDBCmd*>::iterator iter = cmds_.begin(); iter != cmds_.end(); ++iter)
	{
		delete (*iter);
	}
	cmds_.clear();
}

void DBSCtrl::post_db_cmd(BaseDBCmd* pcmd)
{
	ThreadLockWrapper wrapper(lock_);

	cmds_.push_back(pcmd);
}

void DBSCtrl::post_head_db_cmd(BaseDBCmd* pcmd)
{
	ThreadLockWrapper wrapper(lock_);

	cmds_.push_front(pcmd);
}

BaseDBCmd* DBSCtrl::pop_db_cmd()
{
	if (cmds_.size() == 0)
		return 0;

	ThreadLockWrapper wrapper(lock_);

	if (cmds_.size() == 0)
		return 0;

	BaseDBCmd* pcmd = cmds_.front();

	cmds_.pop_front();
	return pcmd;
}

#include <gameLib/redis/global_redis_const.h>

S_INT_64 DBSCtrl::gen_dbid(const char* tablename, RedisClient* rdv)
{
	return rdv->gen_dbid_from_redis(rdkey::global::GLOBAL_IDGEN_HKEY, rdkey::global::GLOBAL_IDGEN_DBID, tablename);
}
