// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __COMMANDLISTENER_H__
#define __COMMANDLISTENER_H__

#pragma pack( push, 8)
#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
#pragma  pack( pop)

#include <google/protobuf/message.h>

#include <map>
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/prolib/BasicProtocol.h>

typedef boost::function< void(NetProtocol*, bool&)>		NETMSG_FUN_MAP;

#define REGISTERMSG( msgid, msgfun, cpoint)	RegistMessage((int)msgid,boost::bind( msgfun, cpoint, boost::placeholders::_1, boost::placeholders::_2));

class MessageProcess
{
public:
	virtual void    InitNetMessage() = 0;
	virtual void    NetProcessMessage(NetProtocol* message, bool& autorelease);

protected:

	virtual void    ProcessMessage(NetProtocol* message, bool& autorelease) = 0;

	void RegistMessage( int msgid, NETMSG_FUN_MAP fun);

private:
	std::map< int, NETMSG_FUN_MAP> funs_;
};

EW_INLINE void MessageProcess::RegistMessage( int msgid, NETMSG_FUN_MAP fun)
{
	funs_[msgid] = fun;
}

EW_INLINE void MessageProcess::NetProcessMessage(NetProtocol* message, bool& autorelease)
{
	if( message == 0)
		return;

	std::map< int, NETMSG_FUN_MAP>::iterator it = funs_.find( (int)message->get_msg());
	if( it != funs_.end())
		it->second( message, autorelease);
	else
		ProcessMessage( message, autorelease);
}

#endif //__COMMANDLISTENER_H__
