#ifndef __GATESERVICELINKFROM_H__
#define __GATESERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

class GateServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	GateServiceLinkFrom():NetLinkFromBase<RouterSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();
};

#endif	//__GATESERVICELINKFROM_H__
