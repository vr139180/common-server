#ifndef __GATESERVICELINKFROM_H__
#define __GATESERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/HomeSession.h"

class GateServiceLinkFrom : public NetLinkFromBase<HomeSession>
{
public:
	GateServiceLinkFrom():NetLinkFromBase<HomeSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();

protected:
};

#endif	//__GATESERVICELINKFROM_H__
