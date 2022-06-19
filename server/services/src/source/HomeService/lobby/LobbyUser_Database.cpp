#include "lobby/LobbyUser.h"

#include <cmsLib/redis/RedisClient.h>

#include <gameLib/redis/user_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/LogExt.h>

#include "HomeServiceApp.h"

void LobbyUser::on_db_rolelist_update(bool initorupdate, UserRoles& from)
{
	if (initorupdate)
	{
		if (cur_state_ != UserState::UserState_RolesLoading)
			return;

		cur_state_ = UserState::UserState_RolesReady;
	}

	this->roles_data_.reset_data();
	this->roles_data_.data_copyall(from);

	//update redis
	RedisClient *rdv = svrApp.get_redisclient();
	roles_data_.update_redis_cache(user_iid_, rdv);

	//notify user
	PRO::User_RoleList_ack *ack = new PRO::User_RoleList_ack();
	set_usertoken(ack);
	ack->mutable_roles()->CopyFrom(roles_data_.roles_data_);

	svrApp.send_protocol_to_gate(ack);

	logDebug(out_service, "send rolelist:%d to user....", roles_data_.roles_data_.roles_size());
}
