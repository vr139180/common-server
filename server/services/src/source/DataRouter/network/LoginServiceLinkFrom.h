#ifndef __LOGINSERVICELINKFROM_H__
#define __LOGINSERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

class LoginServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	LoginServiceLinkFrom();
	virtual ~LoginServiceLinkFrom();

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

#endif	//__LOGINSERVICELINKFROM_H__
