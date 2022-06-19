#ifndef __GATESERVICELINKFROM_H__
#define __GATESERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/UnionSession.h"

class GateServiceLinkFrom : public NetLinkFromBase<UnionSession>
{
public:
	GateServiceLinkFrom():NetLinkFromBase<UnionSession>(),sn_(0){}

	virtual int get_serverindex(){ return 0; }
	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();

protected:
	int sn_;
};

#endif	//__GATESERVICELINKFROM_H__
