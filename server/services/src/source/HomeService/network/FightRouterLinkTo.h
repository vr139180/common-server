#ifndef __FIGHTROUTERLINKTO_H__
#define __FIGHTROUTERLINKTO_H__

#include <cmsLib/net/LinkToBase.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

class FightRouterLinkTo : public LinkToBase
{
public:
	FightRouterLinkTo();
	FightRouterLinkTo(ServiceNodeInfo* pnode);

	virtual void connect();

	void reset(ServiceNodeInfo* pnode);
	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_TIMESTAMP get_token() { return node_->token; }

	virtual std::string get_ext_bykey(const char* key);
	void heart_beat();

	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	void send_netprotocol(PRO::ERK_SERVICETYPE to, BasicProtocol* msg);
	void send_netprotocol(PRO::ERK_SERVICETYPE to, NetProtocol* pro);

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);
	virtual NetProtocol* get_livekeep_msg();

public:
    void on_connected( bool success);
    void on_authed( bool success);
    void on_disconnected();

protected:
	std::shared_ptr<ServiceNodeInfo>	node_;

	//–≠“ÈÕ∑
	SProtocolHead	s_head_;
};

#endif	//__FIGHTROUTERLINKTO_H__
