#ifndef __HOMESESSION_H__
#define __HOMESESSION_H__

#include <cmsLib/net/NetSession.h>
#include <cmsLib/net/NetSessionBindEvent.h>
#include <gameLib/commons/NetLinkFromBase.h>

enum HomeSessionState
{
	HomeSessionState_None,
	HomeSessionState_Closing,
	HomeSessionState_Initialing,
	HomeSessionState_Authed,
};

class HomeSession : public NetSessionBindEvent
{
public:
	HomeSession();
	virtual ~HomeSession();

	boost::shared_ptr<NetSession> get_session() { return session_; }

	void set_netlinkbase(NetLinkFromBase<HomeSession>* p) { parent_ = p; }

	void send_protocol(BasicProtocol* pro);

	void reset();
	void force_close();

	void prepare();
	void auth();
	bool is_auth() { return state_ == HomeSessionState_Authed; }

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
	HomeSessionState				state_;
	S_INT_64						auth_timestamp_;

	boost::shared_ptr<NetSession>	session_;
	NetLinkFromBase<HomeSession>*	parent_;
};

#endif //__HOMESESSION_H__