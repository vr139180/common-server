#ifndef __CHATSERVICELINKFROM_H__
#define __CHATSERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

//chatservice À©Õ¹²ÎÊý
#define CHATSVR_CHATHASH_EXT "chathash"

class ChatServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	ChatServiceLinkFrom():NetLinkFromBase<RouterSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();

	int get_chathash();
};

#endif	//__CHATSERVICELINKFROM_H__
