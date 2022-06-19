#ifndef __GAMEPLAYER_H__
#define __GAMEPLAYER_H__

#include <cmsLib/prolib/core_type.h>
#include <gameLib/commons/NetLinkFromBase.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "network/GateSession.h"

typedef enum tagPlayerState {
	PlayerState_Free = 0,
	PlayerState_Login,
	PlayerState_RoleSel,
	PlayerState_RoleReady,
}PlayerState;

class GamePlayer : public NetLinkFromBase<GateSession>
{
public:
	GamePlayer();
	void init(int s);

	void reuse();
	void preuse(S_INT_64 token, S_INT_64 uid, S_INT_64 gateiid);

	bool is_samesession(S_INT_64 utoken);
	bool is_samesession(S_INT_64 uid, S_INT_64 utoken);

	bool set_usertoken(BasicProtocol* msg);

	int get_userslot() { return slot_; }
	void set_userslot(int s) { slot_ = s; }
	S_INT_64 get_slottoken() { return slottoken_; }
	S_INT_64 get_giduid() { return giduid_; }
	void set_proxystamp(S_INT_64 st) { proxy_stamp_ = st; }
	S_INT_64 get_proxystamp() { return proxy_stamp_; }

	bool is_in_rolerange() { return (cur_state_ >= PlayerState::PlayerState_Login && cur_state_ < PlayerState::PlayerState_RoleReady); }
	bool is_roleready() { return cur_state_ >= PlayerState::PlayerState_RoleReady; }
	S_INT_64 get_roleiid() { return this->role_iid_; }
	//角色选择确定后切换giduid为gateid+role_iid_
	void role_selected_done(S_INT_64 rid, S_INT_64 gateiid);

public:
	virtual void force_linkclose();

	virtual void registinfo_tolog(bool bregist = true);
	//---------------implement NetSessionBindEvent---------------
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);

protected:
	PlayerState	cur_state_;
	S_INT_64	role_iid_;
	//开始使用时间
	S_INT_64	start_timestamp_;
	//加密后的token
	S_INT_64	slottoken_;
	//gidui在角色选择成功后需要重新构建
	S_INT_64	giduid_;

private:
	//所在的slot
	int			slot_;
	S_INT_64	proxy_stamp_;
};

inline 
bool GamePlayer::is_samesession(S_INT_64 utoken)
{
	return (this->slottoken_ == utoken);
}

inline
bool GamePlayer::is_samesession(S_INT_64 uid, S_INT_64 stoken)
{
	return this->get_iid() == uid && this->slottoken_ == stoken;
}

#endif //__GAMEPLAYER_H__
