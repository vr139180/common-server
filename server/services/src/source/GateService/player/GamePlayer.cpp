#include "player/GamePlayer.h"

#include <cmsLib/base/OSSystem.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

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

void GamePlayer::proxy()
{
	cur_state_ = PlayerState::PlayerState_Loginning;
	this->start_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void GamePlayer::auth(S_INT_64 token, S_INT_64 uid, S_INT_64 gateiid)
{
	cur_state_ = PlayerState_Logon;

	s_head_.set_token_slottoken(slot_, token);
	s_head_.set_token_gidrid(gateiid, uid);
}

void GamePlayer::role_selected_done(S_INT_64 rid, S_INT_64 gateiid)
{
	cur_state_ = PlayerState::PlayerState_RoleReady;
	role_iid_ = rid;
	s_head_.set_token_gidrid(gateiid, this->role_iid_);
}

void GamePlayer::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	session_->send_protocol(pro);
}
