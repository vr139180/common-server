#ifndef __NETSESSIONBINDEVENT_H__
#define __NETSESSIONBINDEVENT_H__

#include <cmsLib/prolib/BasicProtocol.h>

class NetSessionBindEvent
{
public:
	//only for connect to client
	virtual void on_cant_connectedto() = 0;
	virtual void on_connectedto_done() = 0;

	virtual void on_connect_lost_netthread() =0;
	virtual void on_recv_protocol_netthread( S_UINT_16 proiid, BasicProtocol* pro) =0;

	virtual BasicProtocol* get_livekeep_msg() { return 0; }
};

#endif	//__NETSESSIONBINDEVENT_H__
