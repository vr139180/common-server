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

#ifndef __PLAYERCHANNEL_H__
#define __PLAYERCHANNEL_H__

#include <vector>
#include <list>
#include <boost/unordered_map.hpp>

#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>

#include "player/GamePlayer.h"

class GamePlayerCtrl;

class PlayerChannel : public VirtualMainThread, public MessageProcess
{
	typedef boost::unordered::unordered_map<S_INT_64, GamePlayer*> ONLINE_PLAYER_MAP;

public:
	PlayerChannel();
	virtual ~PlayerChannel();

	void init_channel(GamePlayerCtrl* p, int pindex, int nums, std::vector<GamePlayer*>& lus);
	void reset_syscmd();

	void send_msg_to_player(NetProtocol* msg);

	GamePlayer* get_player_byslot(int slot);
	GamePlayer* get_player_frommsg(NetProtocol* pro);
	GamePlayer* get_player_bytoken(NetProtocol* pro);
	void force_pc_close_player(GamePlayer* puser);

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}

protected:
	void reset_channel(void*);

	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

private:
	//users
	std::vector<GamePlayer*>	users_;
	int piece_index_;
	int piece_start_;
	int piece_end_;

	GamePlayerCtrl*				parent_;

public:
	void on_connection_timeout(GamePlayer* player);

	void on_disconnected_with_player(GamePlayer* player);

	//net command
public:
	void on_pc_userlogin_ack(NetProtocol* message, bool& autorelease);
	void on_pc_userrelogin_ack(NetProtocol* message, bool& autorelease);

	void on_pc_userlogout_ntf(NetProtocol* pro, bool& autorelease);
	void on_pc_userlogout_force_ntf(NetProtocol* pro, bool& autorelease);
	void on_pc_roleselect_ack(NetProtocol* pro, bool& autorelease);

	void on_pc_entergame_ntf(NetProtocol* pro, bool& autorelease);

	//聊天系统，全局聊天嵌入
	void on_pc_broadcast_chat_globalmsg(NetProtocol* pro, bool& autorelease);
};

#endif //__PLAYERCHANNEL_H__
