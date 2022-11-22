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

#include "player/PlayerChannel.h"

#include <gameLib/LogExt.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

PlayerChannel::PlayerChannel() :VirtualMainThread()
,piece_index_(-1)
, piece_start_(0)
, piece_end_(0)
,parent_(0)
{
}

PlayerChannel::~PlayerChannel()
{
}

void PlayerChannel::thread_worker()
{
	OSSystem::mOS->UpdateNowTick();
	u64 st = OSSystem::mOS->GetTicks();

	int sleepstep = 0;
	while (1)
	{
		if (sleepstep >= loop_num_)
		{
			OSSystem::mOS->thread_yield();
			sleepstep = 0;
		}
		++sleepstep;

		if (will_quit_) break;

		//update time
		OSSystem::mOS->UpdateNowTick();

		CommandBase *pCmd = pop_one_cmd();
		std::unique_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loop_num_;
			continue;
		}

		pCmd->run();
	}
}

void PlayerChannel::init_channel(GamePlayerCtrl* p, int pindex, int nums, std::vector<GamePlayer*>& lus)
{
	this->parent_ = p;
	this->piece_index_ = pindex;
	this->piece_start_ = pindex * nums;
	this->piece_end_ = piece_start_ + lus.size() - 1;

	this->users_.clear();

	for (int ii = 0; ii < lus.size(); ++ii)
		users_.push_back(lus[ii]);

	this->InitNetMessage();
}

void PlayerChannel::reset_channel(void*)
{
}

void PlayerChannel::reset_syscmd()
{
	SystemCommand<void>* cmd =
		new SystemCommand<void>(boost::bind(&PlayerChannel::reset_channel, this, this));
	this->regist_syscmd(cmd);
}

GamePlayer* PlayerChannel::get_player_byslot(int slot)
{
	if (slot < piece_start_ || slot > piece_end_)
		return 0;

	return users_[slot-piece_start_];
}

GamePlayer* PlayerChannel::get_player_bytoken(NetProtocol* pro)
{
	int slot = pro->head_.get_token_slot();
	GamePlayer *user = get_player_byslot(slot);
	if (user == 0)
		return 0;
	if (user->is_same_token(pro->head_) == false)
		return 0;

	return user;
}

GamePlayer* PlayerChannel::get_player_frommsg(NetProtocol* pro)
{
	int slot = pro->head_.get_token_slot();
	GamePlayer *user = get_player_byslot(slot);
	if (user == 0)
		return 0;
	if (user->is_samesession( pro->head_) == false)
		return 0;

	return user;
}

void PlayerChannel::force_pc_close_player(GamePlayer* puser)
{
	puser->registinfo_tolog(false);

	puser->reset();

	//¶Ï¿ªÓ³Éä¹ØÏµ
	parent_->return_slot_to_free(puser->get_userslot());
}

void PlayerChannel::send_msg_to_player(NetProtocol* msg)
{
	GamePlayer *p = get_player_byslot( msg->head_.get_token_slot());
	if (p == 0)
	{
		delete msg;
		return;
	}

	p->send_netprotocol(msg);
}

void PlayerChannel::on_connection_timeout(GamePlayer* player)
{
	force_pc_close_player(player);

	logError(out_runtime, "GateService listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}