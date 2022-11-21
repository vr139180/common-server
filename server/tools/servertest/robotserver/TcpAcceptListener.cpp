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

#include "StdAfx.h"
#include "TcpAcceptListener.h"

TcpAcceptListener::TcpAcceptListener(void) :
server_socket_(INVALID_SOCKET),
notify_(0)
{
	WSADATA wsa_data;
	int error = WSAStartup (MAKEWORD (1, 1), &wsa_data);
}

TcpAcceptListener::~TcpAcceptListener(void)
{
	stop_listen();
	WSACleanup();
}

void TcpAcceptListener::set_notify( ITcpUserNotify* notify)
{
	notify_ = notify;
}

void TcpAcceptListener::release()
{
	NS_STL::map<SOCKET, TcpAcceptUser*>::iterator iter = tcp_users.begin();
	for( ; iter != tcp_users.end(); ++iter)
	{
		TcpAcceptUser *puser =iter->second;
		delete puser;
	}
	tcp_users.clear();
}

bool TcpAcceptListener::start_listen( const char* ip, unsigned short port)
{
	if( server_socket_ != INVALID_SOCKET)
		return true;

	if( 0 == port || 0 == ip )
		return false;

	server_socket_ = socket( AF_INET, SOCK_STREAM, 0);
	if( server_socket_ == INVALID_SOCKET)
		return false;

	struct sockaddr_in serv_addr;
	memset( &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family =AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( ip);

	serv_addr.sin_port=	htons( port);

	int reuse = 1;
	::setsockopt( server_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

	u_long block =1;
	::ioctlsocket( server_socket_, FIONBIO, &block);

	if( ::bind( server_socket_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		closesocket( server_socket_);
		server_socket_ =INVALID_SOCKET;
		return false;
	}

	if( ::listen( server_socket_, 1) == SOCKET_ERROR)
	{
		closesocket( server_socket_);
		server_socket_ =INVALID_SOCKET;
		return false;
	}

	return true;
}

void TcpAcceptListener::stop_listen()
{
	release();

	if( server_socket_ != INVALID_SOCKET)
		closesocket( server_socket_);

	server_socket_ =INVALID_SOCKET;
}

void TcpAcceptListener::do_main( DWORD now)
{
	if( server_socket_ == INVALID_SOCKET)
		return;

	NS_STL::list<SOCKET>::iterator it = tcp_close_users.begin();
	while ( it != tcp_close_users.end())
	{
		NS_STL::map<SOCKET, TcpAcceptUser*>::iterator iter = tcp_users.find(*it);
		if ( iter != tcp_users.end())
		{
			TcpAcceptUser* puser = iter->second;
			tcp_users.erase(iter);
			delete puser;
		}
		it = tcp_close_users.erase(it);
	}

	NS_STL::map<SOCKET, TcpAcceptUser*>::iterator iter = tcp_users.begin();
	for( ; iter != tcp_users.end(); ++iter)
	{
		TcpAcceptUser *puser =iter->second;
		puser->do_main( now);
	}

	struct sockaddr_in addr;
	int addrlen =sizeof( addr);
	memset( &addr, 0, addrlen);

	SOCKET sc =::accept( server_socket_, (struct sockaddr*)&addr, &addrlen);
	if( sc == INVALID_SOCKET)
		return;

	TcpAcceptUser *puser =new TcpAcceptUser( sc, this);
	if ( puser)
	{
		puser->peerip_ = inet_ntoa( addr.sin_addr);
		puser->peerport_ = ntohs( addr.sin_port);
		tcp_users[sc] = puser;
	}

	onAccept( sc);
}

void TcpAcceptListener::send_protocol( SOCKET socket, BasicProtocol* p)
{
	NS_STL::map<SOCKET, TcpAcceptUser*>::iterator it = tcp_users.find( socket);
	if ( it != tcp_users.end())
	{
		it->second->send_protocol( p);
	}
}

void TcpAcceptListener::onAccept( SOCKET socket)
{
	if ( notify_)
	{
		notify_->onAccept( socket);
	}
}

void TcpAcceptListener::onClosed( SOCKET socket)
{
	tcp_close_users.push_back( socket);
	if ( notify_)
	{
		notify_->onClosed( socket);
	}
}

void TcpAcceptListener::onRecvData( char * data, int len, SOCKET socket)
{
	if ( notify_)
	{
		notify_->onRecvData( data, len, socket);
	}
}

void TcpAcceptListener::onRecvProtocol(S_UINT_16 proid, BasicProtocol* p, SOCKET socket)
{
	if ( notify_)
	{
		notify_->onRecvProtocol( proid, p, socket);
	}
}

