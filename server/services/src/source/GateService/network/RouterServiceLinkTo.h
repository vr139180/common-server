#ifndef __ROUTERSERVICELINKTO_H__
#define __ROUTERSERVICELINKTO_H__

#include <cmsLib/net/LinkToBase.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

class RouterServiceLinkTo : public LinkToBase
{
public:
	RouterServiceLinkTo();
	RouterServiceLinkTo(ServiceNodeInfo* pnode);

	virtual void connect();

	void reset(ServiceNodeInfo* pnode);
	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_TIMESTAMP get_token() { return node_->token; }

	virtual std::string get_ext_bykey(const char* key);
	void heart_beat();

	virtual void force_linkclose();

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
};

#endif	//__ROUTERSERVICELINKTO_H__
