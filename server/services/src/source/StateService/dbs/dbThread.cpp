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

#include "dbs/dbThread.h"

#include <cppconn/exception.h>
#include <mysqlc++/mysql_driver.h>

#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>

#include "dbs/DBSCtrl.h"
//5h idle
#define MYSQL_ONLINE_ACTIVE	1000*60*60*5

DBThread::DBThread(DBSCtrl* p):parent_(p)
, finished_(false)
{
}

DBThread::~DBThread()
{
}

void DBThread::init()
{
	p_mysql_connection_.reset( parent_->get_new_connection());
	finished_ = false;
}

void DBThread::run()
{
	sql::mysql::get_driver_instance()->threadInit();

	S_INT_64 last_check_ = OSSystem::mOS->GetTimestamp();

	while (!finished_)
	{
		OSSystem::mOS->thread_yield();
		S_INT_64 tnow = OSSystem::mOS->GetTimestamp();

		BaseDBCmd* pcmd = parent_->pop_db_cmd();
		if (pcmd == 0)
		{
			if (last_check_ + MYSQL_ONLINE_ACTIVE < tnow)
			{
				if( connection_active())
					last_check_ = tnow;
			}

			continue;
		}

		if (!p_mysql_connection_->isValid() || p_mysql_connection_->isClosed())
		{
			p_mysql_connection_->reconnect();

			parent_->post_head_db_cmd(pcmd);
			continue;
		}

		pcmd->run_in_db_thread(p_mysql_connection_.get());
		last_check_ = tnow;

		if (pcmd->reused())
			pcmd->dispath_again();
		else
			delete pcmd;
	}

	try {
		p_mysql_connection_->close();
	}
	catch(...){}

	sql::mysql::get_driver_instance()->threadEnd();
}

void DBThread::finish()
{
	finished_ = true;
}

bool DBThread::connection_active()
{
	try 
	{
		if (!p_mysql_connection_->isValid() || p_mysql_connection_->isClosed())
			p_mysql_connection_->reconnect();

		p_mysql_connection_->nativeSQL("select 1 from dual;");
		return true;
	}
	catch (...) {
		return false;
	}
}