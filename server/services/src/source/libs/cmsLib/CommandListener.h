#ifndef __COMMANDLISTENER_H__
#define __COMMANDLISTENER_H__

#pragma pack( push, 8)
#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
#pragma  pack( pop)

#include <google/protobuf/message_lite.h>

#include <map>
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/prolib/BasicProtocol.h>

typedef boost::function< void( BasicProtocol*, bool&)>		NETMSG_FUN_MAP;

#define REGISTERMSG( msgid, msgfun, cpoint)	RegistMessage((int)msgid,boost::bind( msgfun, cpoint, boost::placeholders::_1, boost::placeholders::_2));

class MessageProcess
{
public:
	virtual void    InitNetMessage() = 0;
	virtual void    NetProcessMessage( BasicProtocol* message, bool& autorelease, int msgid);

protected:

	virtual void    ProcessMessage(BasicProtocol* message, bool& autorelease, int msgid) = 0;

	void RegistMessage( int msgid, NETMSG_FUN_MAP fun);

private:
	std::map< int, NETMSG_FUN_MAP> funs_;
};

EW_INLINE void MessageProcess::RegistMessage( int msgid, NETMSG_FUN_MAP fun)
{
	funs_[msgid] = fun;
}

EW_INLINE void MessageProcess::NetProcessMessage(BasicProtocol* message, bool& autorelease, int msgid)
{
	if( message == 0)
		return;

	std::map< int, NETMSG_FUN_MAP>::iterator it = funs_.find( msgid);
	if( it != funs_.end())
		it->second( message, autorelease);
	else
		ProcessMessage( message, autorelease, msgid);
}

#endif //__COMMANDLISTENER_H__
