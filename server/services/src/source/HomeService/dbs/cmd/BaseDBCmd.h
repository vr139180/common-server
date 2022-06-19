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
#include <cmsLib/prolib/core_type.h>

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
			std::auto_ptr<sql::ResultSet> rs;

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
