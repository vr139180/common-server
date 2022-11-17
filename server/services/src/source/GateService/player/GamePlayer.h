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

#ifndef __GAMEPLAYER_H__
#define __GAMEPLAYER_H__

#include <cmsLib/core_type.h>
#include <cmsLib/net/NetSession.h>
#include <cmsLib/net/NetSessionBindEvent.h>

typedef enum tagPlayerState {
	PlayerState_Free = 0,
	PlayerState_Closing,
	PlayerState_Loginning,	//登陆中
	PlayerState_Sort,		//排队等待
	PlayerState_Logon,
	PlayerState_RoleSel,
	PlayerState_RoleReady,
}PlayerState;

class GamePlayer : public NetSessionBindEvent
{
public:
	GamePlayer();
	virtual ~GamePlayer();

	void init(int s);
	void reset();
	void force_close();

	bool is_samesession(S_INT_64 utoken);
	bool is_samesession(S_INT_64 uid, S_INT_64 utoken);
	bool is_samesession(const SProtocolHead& head);

	int get_userslot() { return slot_; }
	void set_userslot(int s) { slot_ = s; }

	bool is_in_rolerange() { return (cur_state_ >= PlayerState::PlayerState_Logon && cur_state_ < PlayerState::PlayerState_RoleReady); }
	bool is_roleready() { return cur_state_ >= PlayerState::PlayerState_RoleReady; }
	S_INT_64 get_roleiid() { return this->role_iid_; }
	//角色选择确定后切换giduid为gateid+role_iid_
	void role_selected_done(S_INT_64 rid, S_INT_64 gateiid);

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_;}

	void send_netprotocol(NetProtocol* pro) { session_->send_protocol(pro); }
	void send_netprotocol(BasicProtocol* msg);

	S_INT_64 get_iid() { return role_iid_; }

public:
	boost::shared_ptr<NetSession> get_session() { return session_; }

	void send_protocol(NetProtocol* pro) { session_->send_protocol(pro); }

	virtual void force_linkclose();
	virtual void registinfo_tolog(bool bregist = true);

	void heart_beat() {
		if (session_.get() != 0)
			session_->heart_beat();
	}

	void reuse();
	void pre_start();
	S_INT_64 get_starttime() { return start_timestamp_;}
	//完成验证
	void auth(S_INT_64 token, S_INT_64 uid, S_INT_64 gateiid);
	bool is_auth() { return cur_state_ > PlayerState_Loginning; }

	void update(u64 tnow);

protected:
	PlayerState	cur_state_;
	S_INT_64	role_iid_;
	//开始使用时间
	S_INT_64	start_timestamp_;

private:
	CProtocolHead	c_head_;
	SProtocolHead	s_head_;

	//所在的slot
	int			slot_;
	boost::shared_ptr<NetSession>	session_;

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto() {}
	virtual void on_connectedto_done() {}

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(NetProtocol* pro);
	virtual NetProtocol* get_livekeep_msg();

	virtual NetProtocol* unpack_protocol(S_UINT_8 *pbuf, S_UINT_32 prolen);
	virtual bool pack_protocol(S_UINT_8 *pbuf, S_UINT_32 maxlen, NetProtocol* pro, S_UINT_32& prolen);
	virtual bool is_need_unpack_protocol(S_UINT_16 msgid);

};

inline 
bool GamePlayer::is_samesession(S_INT_64 utoken)
{
	return (this->s_head_.get_token_token() == utoken);
}

inline
bool GamePlayer::is_samesession(S_INT_64 uid, S_INT_64 stoken)
{
	return get_iid() == uid && this->s_head_.get_token_token() == stoken;
}

inline
bool GamePlayer::is_samesession(const SProtocolHead& head)
{
	return s_head_.is_same_session( head);
}

#endif //__GAMEPLAYER_H__
