#include "lobby/LobbyUser.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

bool LobbyUser::sync_rolelist()
{
	RedisClient* rds = svrApp.get_redisclient();
	return roles_data_.load_from_redis(user_iid_, rds);
}

