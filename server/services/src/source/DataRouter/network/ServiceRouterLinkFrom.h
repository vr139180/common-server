#ifndef __SERVICEROUTERLINKFROM_H__
#define __SERVICEROUTERLINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

class ServiceRouterLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	ServiceRouterLinkFrom();
	virtual ~ServiceRouterLinkFrom();

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);
	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

public:
	void send_netprotocol(BasicProtocol* msg);

protected:
	SProtocolHead	s_head_;
};

#endif	//__SERVICEROUTERLINKFROM_H__
