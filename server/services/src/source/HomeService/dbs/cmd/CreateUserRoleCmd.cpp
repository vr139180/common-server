#include "dbs/cmd/CreateUserRoleCmd.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/LogExt.h>
#include "lobby/LobbyUser.h"

#include "HomeServiceApp.h"

CreateUserRoleCmd::CreateUserRoleCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p) :BaseDBCmd(uid, token,p)
, success_(false)
{

}

void CreateUserRoleCmd::run_in_db_thread(sql::Connection* p_connection)
{
	bool bauto = p_connection->getAutoCommit();

	std::unique_ptr< sql::PreparedStatement > prep_stmt;
	try
	{
		p_connection->setAutoCommit(false);

		int column = 0;
		//------------------------------------------------------------------------------------------------
		prep_stmt.reset(p_connection->prepareStatement(
			"insert into role_baseinfo(ver_,role_iid,user_iid,nickname,registime,levels) values(?,?,?,?,from_unixtime(?),1);"));
		column = 1;
		prep_stmt->setUInt(column++, base_data_.data().ver_());

		prep_stmt->setInt64(column++, base_data_.data().role_iid());
		prep_stmt->setInt64(column++, base_data_.data().user_iid());
		prep_stmt->setString(column++, base_data_.data().nickname().c_str());
		prep_stmt->setInt(column++, base_data_.data().registime());

		prep_stmt->execute();

		prep_stmt.reset(p_connection->prepareStatement(
			"insert into user_home(ver_,role_iid,home_name,ground_resid,look_at,geo_pos,reside_time,last_residedate,levels) values(?,?,?,?,?,?,?,from_unixtime(?),1);"));
		column = 1;
		prep_stmt->setUInt(column++, home_data_.data().ver_());

		prep_stmt->setInt64(column++, home_data_.data().role_iid());
		prep_stmt->setString(column++, home_data_.data().home_name().c_str());
		prep_stmt->setInt64(column++, home_data_.data().ground_resid());
		prep_stmt->setString(column++, home_data_.data().look_at().c_str());
		prep_stmt->setString(column++, home_data_.data().geo_pos().c_str());
		prep_stmt->setInt(column++, home_data_.data().reside_time());
		prep_stmt->setInt(column++, home_data_.data().last_residedate());

		prep_stmt->execute();

		//------------------------------------------------------------------------------------------------
		prep_stmt.reset(p_connection->prepareStatement(
			"select ver_, role_iid,user_iid,nickname,unix_timestamp(registime),levels from role_baseinfo where user_iid = ?;"));
		prep_stmt->setInt64(1, user_iid_);

		std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
		roles_data_.load_from_database(*res.get());

		release_dboperator(prep_stmt.get());

		p_connection->commit();
		success_ = true;
	}
	catch (sql::SQLException& err)
	{
		release_dboperator(prep_stmt.get());
		p_connection->rollback();

		logError(out_runtime, "database CreateUserRoleCmd -> call insert role_baseinfo error:%s", err.what());
	}

	p_connection->setAutoCommit(bauto);
}

void CreateUserRoleCmd::run()
{
	LobbyUser* puser = lobby_->get_userofsame_from_x(user_iid_, protoken_);
	if (puser == 0) return;

	PRO::User_RoleCreate_ack *ack = new PRO::User_RoleCreate_ack();
	puser->set_usertoken(ack);

	ack->set_result(0);
	if (!success_)
		ack->set_result(2); //´´½¨Ê§°Ü
	else
		ack->set_role_iid(base_data_.data().role_iid());

	svrApp.send_protocol_to_gate(ack);

	if (success_)
	{
		puser->on_db_rolelist_update(false, roles_data_);
	}
}

USED_REDISKEY_USER_NS

void CreateUserRoleCmd::ls_create_new_role(const char* nickname)
{
	RedisClient* rdv = svrApp.get_redisclient();

	base_data_.new_rolebaseinfo(rdv, user_iid_, nickname);
	home_data_.new_userhome( base_data_.data().ver_(), base_data_.data().role_iid(), nickname, rdv);
}
