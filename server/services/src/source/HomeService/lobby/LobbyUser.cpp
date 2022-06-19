#include "lobby/LobbyUser.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "dbs/DBSCtrl.h"

#include "dbs/cmd/CreateUserRoleCmd.h"
#include "dbs/cmd/LoadUserRolesCmd.h"
#include "dbs/cmd/LoadUserInfoCmd.h"

#include "lobby/LobbyService.h"

#include "HomeServiceApp.h"

LobbyUser::LobbyUser() :slot_(0), owner_(0)
, cur_state_(UserState::UserState_Free)
{
}

LobbyUser::~LobbyUser()
{
}

void LobbyUser::init_user(S_INT_64 giduid, S_INT_64 slottoken)
{
	this->rest_user();

	S_INT_64 uid = 0;
	ProtoTokenUtil::parse_usergate2(giduid, uid);
	this->set_user_iid(uid);
	this->set_giduid(giduid);
	this->set_slottoken(slottoken);

	this->cur_state_ = UserState::UserState_Free;
	
	if (this->sync_rolelist() == false)
	{
		this->cur_state_ = UserState::UserState_RolesLoading;

		//load from database
		BaseDBCmd* pcmd = new LoadUserRolesCmd(user_iid_, slottoken_, owner_);
		dbsStore->post_db_cmd(pcmd);
	}
	else
	{
		this->cur_state_ = UserState::UserState_RolesReady;

		//nootify user
		PRO::User_RoleList_ack *ack = new PRO::User_RoleList_ack();
		set_usertoken(ack);

		ack->set_allocated_roles( roles_data_.clone_data<PRO::DBUserRoles>());

		svrApp.send_protocol_to_gate(ack);
	}
}

void LobbyUser::rest_user()
{
	this->cur_state_ = UserState::UserState_Free;
	
	this->roles_data_.reset_data();
	this->reset_usercache();
}

bool LobbyUser::set_usertoken(BasicProtocol* msg)
{
	return ProtoUtil::set_usertokenx(msg, giduid_, slottoken_);
}
