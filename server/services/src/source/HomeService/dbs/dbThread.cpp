#include "dbs/dbThread.h"

#include <cppconn/exception.h>
#include <mysqlc++/mysql_driver.h>

#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>

#include "dbs/DBSCtrl.h"

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

	while (!finished_)
	{
		OSSystem::mOS->thread_yield();

		BaseDBCmd* pcmd = parent_->pop_db_cmd();
		if (pcmd == 0)
			continue;

		if (p_mysql_connection_.get() == 0 || p_mysql_connection_->isClosed())
		{
			p_mysql_connection_->reconnect();

			parent_->post_head_db_cmd(pcmd);
			continue;
		}

		pcmd->run_in_db_thread(p_mysql_connection_.get());
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
