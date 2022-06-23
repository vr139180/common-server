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
	std::shared_ptr<sql::Connection>	p_mysql_connection_;
	DBSCtrl*							parent_;
	bool								finished_;
};

#endif //__DBTHREAD_H__