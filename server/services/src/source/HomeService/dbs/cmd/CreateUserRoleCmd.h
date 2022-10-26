#ifndef __CREATEUSERROLECMD_H__
#define __CREATEUSERROLECMD_H__

#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserRoles.h"
#include "lobby/user/UserBuildings.h"
#include "lobby/user/UserPets.h"
#include "lobby/user/UserBase.h"
#include "lobby/user/UserHome.h"

#include "dbs/cmd/BaseDBCmd.h"

class CreateUserRoleCmd : public BaseDBCmd
{
public:
	CreateUserRoleCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p);

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection);
	//run in logic thread
	virtual void run();

	virtual bool reused() { return true; }

	void ls_create_new_role(const char* nickname);

protected:
	//datas
	UserBase		base_data_;
	UserHome		home_data_;

	//result
	UserRoles	roles_data_;
	bool		success_;
};

#endif //__CREATEUSERROLECMD_H__
