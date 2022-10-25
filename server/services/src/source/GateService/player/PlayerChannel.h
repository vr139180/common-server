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

	void send_msg_to_player(NetProtocol* msg, int slot);

	GamePlayer* get_player_byslot(int slot);
	GamePlayer* get_player_byslot(int slot, S_INT_64 token);
	GamePlayer* get_player_frommsg(NetProtocol* pro);
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
	//�ɷ���slot����
	int volatile				online_user_nums_;
	ONLINE_PLAYER_MAP			online_users_;

	//users
	std::vector<GamePlayer*>	users_;
	int piece_index_;
	int piece_start_;
	int piece_end_;

	GamePlayerCtrl*				parent_;

public:
	void on_connection_timeout(GamePlayer* player);

	void on_disconnected_with_player(GamePlayer* player);
	void on_player_login(GamePlayer* player);

	//net command
public:
	void on_cth_userproxylogin_req(NetProtocol* message, bool& autorelease, void* p);

	void on_pc_userlogout_ntf(NetProtocol* pro, bool& autorelease);
	void on_pc_roleselect_ack(NetProtocol* pro, bool& autorelease);

	//����ϵͳ��ȫ������Ƕ��
	void on_pc_broadcast_chat_globalmsg(NetProtocol* pro, bool& autorelease);
};

#endif //__PLAYERCHANNEL_H__
