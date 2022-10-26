#ifndef __STATESERVICELINKFROM_H__
#define __STATESERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

class StateServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	StateServiceLinkFrom():NetLinkFromBase<RouterSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();
};

#endif	//__STATESERVICELINKFROM_H__
