#ifndef __FIGHTROUTERSESSION_H__
#define __FIGHTROUTERSESSION_H__

#include <cmsLib/net/NetSession.h>
#include <cmsLib/net/NetSessionBindEvent.h>
#include <gameLib/commons/NetLinkFromBase.h>

enum SessionState
{
	SessionState_None,
	SessionState_Closing,
	SessionState_Initialing,
	SessionState_Authed,
};

class FightRouterSession : public NetSessionBindEvent
{
public:
	FightRouterSession();
	virtual ~FightRouterSession();

	boost::shared_ptr<NetSession> get_session() { return session_; }

	void set_netlinkbase(NetLinkFromBase<FightRouterSession>* p) { parent_ = p; }

	void send_protocol(BasicProtocol* pro);

	void reset();
	void force_close();

	void prepare();
	void auth();
	bool is_auth() { return state_ == SessionState_Authed; }

	void update(u64 tnow);

	void heart_beat() {
		if (session_.get() != 0)
			session_->heart_beat();
	}

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto() {}
	virtual void on_connectedto_done() {}

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual BasicProtocol* get_livekeep_msg();

protected:
	SessionState					state_;
	S_TIMESTAMP						auth_timestamp_;

	boost::shared_ptr<NetSession>	session_;
	NetLinkFromBase<FightRouterSession>*	parent_;
};

#endif //__FIGHTROUTERSESSION_H__