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

#ifndef __GAMEPLAYERCTRL_H__
#define __GAMEPLAYERCTRL_H__

#include <vector>
#include <list>
#include <set>
#include <boost/unordered_map.hpp>
#include <boost/scoped_array.hpp>
#include <cmsLib/ThreadLock.h>

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include <gameLib/gatehome/GHUsersSeparate.h>
#include "player/GamePlayer.h"
#include "player/PlayerChannel.h"

class GamePlayerCtrl : public GHUsersSeparate<GamePlayer>
{
	typedef GHUsersSeparate<GamePlayer> base;
public:
	GamePlayerCtrl();
	static GamePlayerCtrl& instance();

	void init_gameplayerctrl(int maxplayers);
	void uninit_gameplayerctrl();
	void start();
	void stop();

	void route_msg_to_player(NetProtocol* msg);

	void post_slot_cmd(CommandBase* cmd, int slot);
	PlayerChannel* get_channel_by_slot(int slot);
	PlayerChannel* get_channel_by_head(const SProtocolHead& head);

	GamePlayer* ask_free_slot( S_INT_64 gateid);
	void return_slot_to_free(int slot);
	bool auth_wait_slot(int slot);

	S_INT_32 get_free_slot_num();

public:
	//聊天模块，全局聊天
	void chat_globalmsg_notify(NetProtocol* msg);

public:
	//登陆维护定时器
	void maintnce_proxylogin_timer(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//lobby service
	boost::scoped_array<PlayerChannel>	all_channels_;
	int									channel_nums_;

	std::set<GamePlayer*>	free_slots_;
	volatile int			free_slot_num_;
	std::list<GamePlayer*>	wait_auth_slots_queue_;
	int						wait_auth_slot_num_;
	//同步用sequence编号，只在main thread发生变化
	S_INT_64				slot_sequence_;
	//最后一次请求slot时间
	S_INT_64				last_freeslot_ask_;

	ThreadLock				lock_;
};

#endif //__GAMEPLAYERCTRL_H__
