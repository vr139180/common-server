#ifndef __MAILSERVICELINKFROM_H__
#define __MAILSERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/RouterSession.h"

//mailservice À©Õ¹²ÎÊý
#define MAILSVR_MAILHASH_EXT "mailhash"

class MailServiceLinkFrom : public NetLinkFromBase<RouterSession>
{
public:
	MailServiceLinkFrom():NetLinkFromBase<RouterSession>(){}

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro);
	virtual void force_linkclose();

	int get_mailhash();
};

#endif	//__MAILSERVICELINKFROM_H__
