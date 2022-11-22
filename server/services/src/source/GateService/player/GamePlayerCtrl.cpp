// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "player/GamePlayerCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/global_const.h>

GamePlayerCtrl& GamePlayerCtrl::instance()
{
	static GamePlayerCtrl s_instance;
	return s_instance;
}

GamePlayerCtrl::GamePlayerCtrl():base()
,channel_nums_(0)
, free_slot_num_(0)
, wait_auth_slot_num_(0)
, slot_sequence_(0)
, last_freeslot_ask_(0)
{
}

void GamePlayerCtrl::init_gameplayerctrl( int maxplayers)
{
	this->init_separate(maxplayers, GATE_PIECE_NUM_MAX);

	this->channel_nums_ = this->get_max_piece();
	this->all_channels_.reset(new PlayerChannel[channel_nums_]);
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		std::vector<GamePlayer*> us;
		get_piece_data(ii, us);
		all_channels_[ii].init_channel(this, ii, this->get_piece_slotnum(), us);
	}
	//init freeslot
	free_slots_.clear();
	for (int ii = 0; ii < maxplayers; ++ii)
	{
		free_slots_.insert(get_data_of_slot(ii));
	}

	free_slot_num_ = free_slots_.size();
	wait_auth_slots_queue_.clear();
	wait_auth_slot_num_ = 0;
}

void GamePlayerCtrl::uninit_gameplayerctrl()
{
	all_channels_.reset(0);
	GHUsersSeparate<GamePlayer>::uninit();
}

void GamePlayerCtrl::start()
{
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		all_channels_[ii].init(100);
		all_channels_[ii].start();
	}
}

void GamePlayerCtrl::stop()
{
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		all_channels_[ii].stop();
	}
}

GamePlayer* GamePlayerCtrl::ask_free_slot(S_INT_64 gateid)
{
	ThreadLockWrapper tl(lock_);

	//记录最后一次请求时间
	last_freeslot_ask_ = OSSystem::mOS->GetTimestamp();
	
	if (free_slots_.size() == 0)
		return 0;

	GamePlayer *gp = 0;
	for (std::set<GamePlayer*>::iterator iter = free_slots_.begin(); iter != free_slots_.end();)
	{
		gp = (*iter);
		iter = free_slots_.erase(iter);

		if (std::find(wait_auth_slots_queue_.begin(), wait_auth_slots_queue_.end(), gp) == wait_auth_slots_queue_.end())
			break;

		gp = 0;
	}
	free_slot_num_ = free_slots_.size();

	if (gp != 0)
	{
		wait_auth_slots_queue_.push_back(gp);
		wait_auth_slot_num_ = wait_auth_slots_queue_.size();

		//记录等待开始验证的时间
		gp->pre_start( gateid);

		return gp;
	}

	return 0;
}

void GamePlayerCtrl::return_slot_to_free(int slot)
{
	GamePlayer* p = get_data_of_slot(slot);
	if (p == 0)
		return;

	ThreadLockWrapper tl(lock_);

	free_slots_.insert(p);
	free_slot_num_ = free_slots_.size();

	std::list<GamePlayer*>::iterator ft = std::find(wait_auth_slots_queue_.begin(), wait_auth_slots_queue_.end(), p);
	if (ft != wait_auth_slots_queue_.end())
		wait_auth_slots_queue_.erase(ft);
	wait_auth_slot_num_ = wait_auth_slots_queue_.size();
}

bool GamePlayerCtrl::auth_wait_slot(int slot)
{
	GamePlayer* p = get_data_of_slot(slot);
	if (p == 0)
		return false;

	ThreadLockWrapper tl(lock_);

	std::list<GamePlayer*>::iterator ft = std::find(wait_auth_slots_queue_.begin(), wait_auth_slots_queue_.end(), p);
	if (ft == wait_auth_slots_queue_.end())
		return false;

	wait_auth_slots_queue_.erase(ft);
	wait_auth_slot_num_ = wait_auth_slots_queue_.size();

	return true;
}

PlayerChannel* GamePlayerCtrl::get_channel_by_slot(int slot)
{
	int pindex = get_pieceindex_from_slot(slot);
	if (pindex == -1)
		return 0;

	return &(all_channels_[pindex]);
}

PlayerChannel* GamePlayerCtrl::get_channel_by_head(const SProtocolHead& head)
{
	S_INT_32 slot = head.get_token_slot();
	return get_channel_by_slot(slot);
}

void GamePlayerCtrl::post_slot_cmd(CommandBase* cmd, int slot)
{
	int pindex = get_pieceindex_from_slot(slot);
	if (pindex == -1)
	{
		delete cmd;

		logError(out_runtime, "gate service user slot[%d] is error", slot);
		return;
	}

	all_channels_[pindex].regist_syscmd(cmd);
}

void GamePlayerCtrl::route_msg_to_player(NetProtocol* msg)
{
	PlayerChannel* pchannel = get_channel_by_head(msg->head_);
	if (pchannel == 0)
	{
		delete msg;
		return;
	}

	pchannel->send_msg_to_player(msg);
}
