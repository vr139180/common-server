#ifndef __EUREKASESSION_H__
#define __EUREKASESSION_H__

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

class EurekaSession : public NetSessionBindEvent
{
public:
	EurekaSession();
	virtual ~EurekaSession();

	boost::shared_ptr<NetSession> get_session() { return session_; }

	void set_netlinkbase(NetLinkFromBase<EurekaSession>* p) { parent_ = p; }

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

	void send_protocol(NetProtocol* pro) { session_->send_protocol(pro); }

	void send_to_service(const SProtocolHead& head, BasicProtocol* msg);

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto() {}
	virtual void on_connectedto_done() {}

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);
	virtual NetProtocol* get_livekeep_msg();

protected:
	SessionState					state_;
	S_INT_64						auth_timestamp_;

	boost::shared_ptr<NetSession>	session_;
	NetLinkFromBase<EurekaSession>*	parent_;
};

#endif //__EUREKASESSION_H__