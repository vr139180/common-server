#ifndef __HOMESERVICELINKTO_H__
#define __HOMESERVICELINKTO_H__

#include <cmsLib/net/LinkToBase.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

class HomeServiceLinkTo : public LinkToBase
{
public:
	HomeServiceLinkTo();
	HomeServiceLinkTo(ServiceNodeInfo* pnode);

	virtual void connect();

	void reset(ServiceNodeInfo* pnode, S_INT_64 bindtoken);
	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_TIMESTAMP get_token() { return node_->token; }

	virtual std::string get_ext_bykey(const char* key);
	void heart_beat();

	virtual void force_linkclose();

	void bind_home_confirm();

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual BasicProtocol* get_livekeep_msg();

public:
    void on_connected( bool success);
    void on_authed( bool success);
    void on_disconnected();

protected:
	std::auto_ptr<ServiceNodeInfo>	node_;

	S_INT_64 bind_token_;
};

#endif	//__HOMESERVICELINKTO_H__
