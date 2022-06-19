#include "player/GamePlayer.h"

#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/ProtoUtil.h>

GamePlayer::GamePlayer():NetLinkFromBase<GateSession>()
, slot_(-1)
, cur_state_(PlayerState::PlayerState_Free)
, role_iid_(0)
{
}

void GamePlayer::init(int s)
{
	this->slot_ = s;
	this->cur_state_ = PlayerState::PlayerState_Free;
	this->role_iid_ = 0;
}

void GamePlayer::reuse()
{
	this->reset();
}

void GamePlayer::force_linkclose()
{
	force_close();
}

void GamePlayer::preuse(S_INT_64 token, S_INT_64 uid, S_INT_64 gateiid)
{
	this->set_linkbase_info(uid, token);
	this->slottoken_ = ProtoTokenUtil::build_usertoken(slot_, token);
	this->giduid_ = ProtoTokenUtil::build_usergate( gateiid, uid);

	this->start_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void GamePlayer::role_selected_done(S_INT_64 rid, S_INT_64 gateiid)
{
	cur_state_ = PlayerState::PlayerState_RoleReady;
	role_iid_ = rid;
	this->giduid_ = ProtoTokenUtil::build_usergate(gateiid, this->role_iid_);
}

bool GamePlayer::set_usertoken(BasicProtocol* msg)
{
	return ProtoUtil::set_usertokenx(msg, giduid_, slottoken_);
}
