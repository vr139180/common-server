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

	void send_msg_to_player(BasicProtocol* msg, int slot);

	GamePlayer* ask_free_slot(int slot, S_INT_64 token, S_INT_64 uid);

	GamePlayer* get_player_byslot(int slot);
	GamePlayer* get_player_byslot(int slot, S_INT_64 token);
	GamePlayer* get_player_frommsg(BasicProtocol* msg);
	void force_pc_close_player(GamePlayer* puser);

public:
	//---------------------------MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(BasicProtocol* message, bool& autorelease, int msgid) {}

public:
	void on_cth_freeslot_req(BasicProtocol* msg, bool& autorelease, int slot);
	void on_cth_userproxylogin_req(BasicProtocol* message, bool& autorelease, void* p);

	void on_disconnected_with_player(GamePlayer* p);

protected:
	void reset_channel(void*);

	//---------------------------VirtualMainThread virtual function----------------------
	virtual void thread_worker();

private:
	//可分配slot数量
	int volatile				online_user_nums_;
	ONLINE_PLAYER_MAP			online_users_;

	//users
	std::vector<GamePlayer*>	users_;
	int piece_index_;
	int piece_start_;
	int piece_end_;

	GamePlayerCtrl*				parent_;

	//net command
public:
	void on_pc_userlogout_ntf(BasicProtocol* pro, bool& autorelease);
	void on_pc_roleselect_ack(BasicProtocol* pro, bool& autorelease);

	//聊天系统，全局聊天嵌入
	void on_pc_broadcast_chat_globalmsg(BasicProtocol* pro, bool& autorelease);
};

#endif //__PLAYERCHANNEL_H__
