#include "lobby/LobbyUser.h"

#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>

#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

void LobbyUser::task_data_loaded()
{
	task_resolver_.init_taskresolver( task_data_.task_groups_data_, task_data_.tasks_data_);
}

bool LobbyUser::is_end_of_taskgroup(S_INT_32 gid)
{
	return task_data_.is_taskgroup_end( gid);
}

int LobbyUser::get_role_level()
{
	return base_data_.get_levels();
}

int LobbyUser::get_bag_itemnum(S_INT_64 itemiid)
{
	return 1;
}
