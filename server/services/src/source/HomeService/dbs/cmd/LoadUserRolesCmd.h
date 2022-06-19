#ifndef __LOADUSERROLESCMD_H__
#define __LOADUSERROLESCMD_H__

#include <cmsLib/prolib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include "lobby/user/UserRoles.h"

#include "dbs/cmd/BaseDBCmd.h"

class LoadUserRolesCmd : public BaseDBCmd
{
public:
	LoadUserRolesCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p);

	//run in db thread
	virtual void run_in_db_thread(sql::Connection* p_connection);
	//run in logic thread
	virtual void run();

	virtual bool reused() { return true; }

protected:
	UserRoles	roles_data_;

	bool		success_;
};

#endif //__LOADUSERROLESCMD_H__
