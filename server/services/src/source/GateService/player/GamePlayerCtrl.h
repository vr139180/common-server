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
	typedef boost::unordered::unordered_map<int, S_INT_64> AUTH_MAP;
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

	GamePlayer* ask_free_slot();
	void return_slot_to_free(int slot);
	bool auth_wait_slot(int slot);

public:
	//聊天模块，全局聊天
	void chat_globalmsg_notify(NetProtocol* msg);

public:
	void ask_gateslot_sync();
	//登陆维护定时器
	void maintnce_proxylogin_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void on_mth_gateslot_sync(void*);

protected:
	//lobby service
	boost::scoped_array<PlayerChannel>	all_channels_;
	int									channel_nums_;

	std::set<GamePlayer*>	free_slots_;
	int						free_slot_num_;
	std::list<GamePlayer*>	wait_auth_slots_queue_;
	int						wait_auth_slot_num_;
	//同步用sequence编号，只在main thread发生变化
	S_INT_64				slot_sequence_;
	//最后一次请求slot时间
	S_INT_64				last_freeslot_ask_;

	ThreadLock				lock_;
};

#endif //__GAMEPLAYERCTRL_H__
