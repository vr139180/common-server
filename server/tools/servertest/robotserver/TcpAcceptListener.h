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

#pragma once

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include <map>
#include <list>
#include "TcpAcceptUser.h"

class TcpAcceptListener : ITcpUserNotify
{
public:
	TcpAcceptListener(void);
	virtual ~TcpAcceptListener(void);

	void release();
	
	bool start_listen( const char* ip, unsigned short port);
	void stop_listen();

	void set_notify( ITcpUserNotify* notify);

	void do_main( DWORD now);

	void send_protocol( SOCKET socket, BasicProtocol* p);

protected:
	virtual void onAccept( SOCKET socket);
	virtual void onClosed( SOCKET socket);
	virtual void onRecvData( char * data, int len, SOCKET socket);
	virtual void onRecvProtocol(S_UINT_16 proid, BasicProtocol* p, SOCKET socket);

public:
	NS_STL::map<SOCKET, TcpAcceptUser*> tcp_users;

private:
	NS_STL::list<SOCKET>	tcp_close_users;
	SOCKET					server_socket_;
	ITcpUserNotify*			notify_;
};
