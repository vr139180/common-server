#include "dbs/cmd/LoadUserInfoCmd.h"

#include <gameLib/LogExt.h>
#include "lobby/LobbyUser.h"

#include "HomeServiceApp.h"

LoadUserInfoCmd::LoadUserInfoCmd(S_INT_64 rid, S_INT_64 uid, S_INT_64 token, LobbyService* p):BaseDBCmd(uid,token,p)
, success_(false)
, role_iid_(rid)
{
}

void LoadUserInfoCmd::reuse_cmd(S_INT_64 uid, S_INT_64 token)
{
	this->user_iid_ = uid;
	this->protoken_ = token;
}

void LoadUserInfoCmd::run_in_db_thread(sql::Connection* p_connection)
{
	std::unique_ptr< sql::Statement > prep_stmt;
	try
	{
		prep_stmt.reset(p_connection->createStatement());
		std::stringstream sql;
		sql << "CALL get_role_data(" << role_iid_ << ");";

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery(sql.str().c_str()));
		//role baseinfo
		if (res.get() != 0)
		{
			base_data_.load_from_database(*res.get());
		}

		//home info
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			home_data_.load_from_database(*res.get());
		}

		//building
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			building_data_.load_from_database(*res.get());
		}

		//pets
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			pet_data_.load_from_database(*res.get());
		}

		//tasks
		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			task_data_.load_from_database1(*res.get());
		}

		if (prep_stmt->getMoreResults())
		{
			res.reset(prep_stmt->getResultSet());
			task_data_.load_from_database2(*res.get());
		}

		success_ = true;
	}
	catch (sql::SQLException& err)
	{
		logError(out_sys, "database LoadUserInfoCmd -> call get_role_data produce error:%s", err.what());
	}

	release_dboperator(prep_stmt.get());
}

void LoadUserInfoCmd::run()
{
	if (!success_) return;

	LobbyUser* puser = lobby_->get_userofsame_from_x(user_iid_, protoken_);
	if (puser == 0) return;

	puser->on_db_roledata_sync1( base_data_, home_data_, building_data_, pet_data_, task_data_);
	puser->role_selected_done();
}
