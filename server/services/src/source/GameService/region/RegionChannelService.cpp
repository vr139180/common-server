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

#include "region/RegionChannelService.h"

#include <gameLib/global_const.h>
#include <worldsLib/utils/WorldUtil.h>
#include <worldsLib/GameRegionMeta.h>

#include "config/GameConfig.h"
#include "region/RegionMapBoxImpl.h"

#include "GameServiceApp.h"

RegionChannelService::RegionChannelService() :base(), gameid_( 0)
{
}

RegionChannelService::~RegionChannelService()
{
}

void RegionChannelService::thread_worker()
{
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

void RegionChannelService::init_channel( S_INT_32 cid)
{
	this->channel_index_ = cid;

	this->InitNetMessage();
	channel_users_.init_cap(1000);

	region_map_.reset(new RegionMapBoxImpl( this));
	region_map_->init_region();
}

void RegionChannelService::build_gameid(S_INT_64 serviceid)
{
	gameid_ = WorldUtil::build_gameid(serviceid, channel_index_, GameRegionMeta::instance().get_regionid());
}

bool RegionChannelService::is_max_performance()
{
	return channel_users_.get_online_playernum() >= GAME_CHANNEL_MAXPLAYER;
}

GamePlayer* RegionChannelService::get_userbyid_from_msg(NetProtocol* msg, bool mustexist)
{
	if( mustexist)
		return channel_users_.get_gameuser_exist(msg->get_useriid());
	else
		return channel_users_.get_gameuser(msg->get_useriid());
}
