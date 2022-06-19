#ifndef __FRIENDSERVICELINKFROM_H__
#define __FRIENDSERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

//friendservice À©Õ¹²ÎÊý
#define FRIENDSVR_FRDHASH_EXT "frdhash"

class FriendServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	FriendServiceLinkFrom():NetLinkFromBase<RouterSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();

	int get_frdhash();
};

#endif	//__FRIENDSERVICELINKFROM_H__
