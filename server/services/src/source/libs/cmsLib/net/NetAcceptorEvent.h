#ifndef __NETACCEPTOREVENT_H__
#define __NETACCEPTOREVENT_H__

class NetSession;

class NetAcceptorEvent
{
public:
	typedef boost::shared_ptr< NetSession >		NetSessionPtr;

public:
	virtual NetSessionPtr ask_free_netsession() =0;
	virtual void accept_netsession( NetSessionPtr session, bool refuse, int err) =0;
};

#endif	//__NETACCEPTOREVENT_H__
