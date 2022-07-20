#include "lobby/LobbyUser.h"

#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

void LobbyUser::task_data_loaded()
{
	task_resolver_.init_taskresolver( task_data_.task_groups_data_, task_data_.tasks_data_);
}

bool LobbyUser::is_end_of_taskgroup(S_INT_32 gid)
{
	//TODO:
	return false;
}

int LobbyUser::get_role_level()
{
	return 0;
}

int LobbyUser::get_bag_itemnum(S_INT_64 itemiid)
{
	return 0;
}

//-----------------------------------------------------------------------------------------
void LobbyUser::notify_new_taskgroup( TaskGroupCellRT* gcrt)
{
	this->task_new_group(gcrt);
}

void LobbyUser::notify_forward_nextcell_taskgroup(TaskGroupCellRT* gcrt)
{

}
