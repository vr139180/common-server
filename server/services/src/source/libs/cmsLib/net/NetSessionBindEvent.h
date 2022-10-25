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
	virtual void on_recv_protocol_netthread( NetProtocol* pro) =0;

	virtual NetProtocol* get_livekeep_msg() { return 0; }

	//Ω‚Œˆ–≠“È
	virtual NetProtocol* unpack_protocol(S_UINT_8 *pbuf, S_UINT_32 prolen);
	virtual bool pack_protocol(S_UINT_8 *pbuf, S_UINT_32 maxlen, NetProtocol* pro, S_UINT_32& prolen);
	virtual bool is_need_unpack_protocol(S_UINT_16 msgid) { return true; }
};

#endif	//__NETSESSIONBINDEVENT_H__
