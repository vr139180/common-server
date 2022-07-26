#include "dbs/cmd/LoadUserRolesCmd.h"

#include <gameLib/LogExt.h>
#include "lobby/LobbyUser.h"

#include "HomeServiceApp.h"

LoadUserRolesCmd::LoadUserRolesCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p) :BaseDBCmd(uid, token, p)
, success_(false)
{

}

void LoadUserRolesCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::Statement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->createStatement());

		std::stringstream sql;
		sql << "select ver_,role_iid,user_iid,nickname,unix_timestamp(registime) from role_baseinfo where user_iid = " << user_iid_ << ";";

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery(sql.str().c_str()));
		//role list
		if (res.get() != 0)
		{
			sql::ResultSet* rss = res.get();
			roles_data_.load_from_database(*rss);

			success_ = true;
		}
	}
	catch (sql::SQLException& err)
	{
		logError(out_sys, "database LoadUserRolesCmd -> call select from role_baseinfo produce error:%s", err.what());
	}

	release_dboperator(prep_stmt.get());
}

void LoadUserRolesCmd::run()
{
	if (!success_)
		return;

	LobbyUser* puser = lobby_->get_userofsame_from_x(user_iid_, protoken_);
	if (puser == 0) return;

	puser->on_db_rolelist_update( true, roles_data_);
}
