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

#ifndef __REGIONCHANNELSERVICE_H__
#define __REGIONCHANNELSERVICE_H__

#include <boost/scoped_ptr.hpp>
#include <cmsLib/CommandListener.h>
#include <cmsLib/system/VirtualMainThread.h>
#include <cmsLib/system/TimerContainer.h>

#include "region/GamePlayer.h"
#include "region/RegionPlayerContainer.h"
#include "region/IRegionMap.h"

class RegionChannelService : public VirtualMainThread, public MessageProcess
{
	typedef VirtualMainThread base;
public:
	RegionChannelService();
	virtual ~RegionChannelService();

	void init_channel( S_INT_32 cid);
	void build_gameid(S_INT_64 serviceid);

	S_INT_64 get_gameid() { return gameid_; }
	RegionPlayerContainer<GamePlayer>& get_users() { return channel_users_; }

	bool is_max_performance();

public:
	GamePlayer* get_userbyid_from_msg(NetProtocol* msg, bool mustexist = true);

private:
	RegionPlayerContainer<GamePlayer>	channel_users_;

	boost::scoped_ptr<IRegionMap>		region_map_;

	//±àÂëºóµÄgameid
	S_INT_64	gameid_;
	S_INT_32	channel_index_;

	TimerContainer	channel_timer_;

protected:
	void regist_timer();

	void on_userlive_check_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void on_userinout_check_timer(u64 tnow, int interval, u64 iid, bool& finish);

	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}

protected:
	void on_gate_enter_game_req(NetProtocol* pro, bool& autorelease);
	void on_gate_userlive_ntf(NetProtocol* pro, bool& autorelease);
	void on_home_mysimpleinfo_ack(NetProtocol* pro, bool& autorelease);
	void on_state_userlogout_ntf(NetProtocol* pro, bool& autorelease);

	//-------------------------------------------------------------------
	void on_pl_userstate_sync(NetProtocol* pro, bool& autorelease);
};

#endif //__REGIONCHANNELSERVICE_H__
