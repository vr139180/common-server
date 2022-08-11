#pragma once

#include <list>

#include <cmsLib/prolib/core_type.h>
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

	NS_STL::list<BasicProtocol *> send_pro_pools_;
	char recv_buffer_[ROBOT_SOCKET_BUF];
	int data_len_;
};
