#include "lobby/LobbyService.h"

#include <gameLib/redis/user_redis_const.h>
#include "dbs/DBSCtrl.h"
#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

ScriptContext* LobbyService::get_lua_context()
{
	return &lua_context_;
}

S_INT_64 LobbyService::new_taskgroup_iid()
{
	RedisClient* rdv = svrApp.get_redisclient();
	S_INT_64 iid = dbsStore->gen_dbid(DBID_USER_TASKGROUP, rdv);
	return iid;
}

S_INT_64 LobbyService::new_task_iid()
{
	RedisClient* rdv = svrApp.get_redisclient();
	S_INT_64 iid = dbsStore->gen_dbid(DBID_USER_TASKINFO, rdv);
	return iid;
}
