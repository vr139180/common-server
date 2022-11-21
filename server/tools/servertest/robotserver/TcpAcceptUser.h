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

#include <list>

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#define ROBOT_SOCKET_BUF		1024*512	//512k

class ITcpUserNotify
{
public:
	virtual void onAccept( SOCKET socket)=0;
	virtual void onClosed( SOCKET socket)=0;
	virtual void onRecvData( char * data, int len, SOCKET socket)=0;
	virtual void onRecvProtocol(S_UINT_16 proid, BasicProtocol* p, SOCKET socket){};
};

class TcpAcceptUser
{
public:
	TcpAcceptUser( SOCKET socket, ITcpUserNotify* notify);
	virtual ~TcpAcceptUser(void);

	void do_main( DWORD now);
	void send_protocol( BasicProtocol* p);

private:
	void close();
	bool send_protocol();
	bool recv_protocol();

public:
	NS_STL::string	peerip_;
	unsigned short	peerport_;

private:
	SOCKET			socket_;
	ITcpUserNotify* notify_;

	NS_STL::list<BasicProtocol *>	send_pro_pools_;
	char	recv_buffer_[ROBOT_SOCKET_BUF];
	int		data_len_;

	CProtocolHead	head_;
};
