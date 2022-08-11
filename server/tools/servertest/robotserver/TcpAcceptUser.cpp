#include "StdAfx.h"

#include <gameLib/protobuf/Proto_all.h>

#include "TcpAcceptUser.h"
#include "OptionUtil.h"

#define MAX_PACK_LEN	16*1024
#define SINGLE_PACK_LEN 1024*16

#define NET_HEAD_MASK_ZERO_H	0x0000FFFF
#define NET_HEAD_MASK_ZERO_L	0xFFFF0000

#define MAKE_NETHEAD( VAL, PROID, LENS) { VAL = LENS; VAL = (VAL << 16)&NET_HEAD_MASK_ZERO_L; VAL = VAL|(PROID&NET_HEAD_MASK_ZERO_H);}
#define GET_NETHEAD( VAL, PROID, LENS) { LENS = (S_UINT_16)((VAL&NET_HEAD_MASK_ZERO_L)>>16); PROID = (S_UINT_16)(VAL&NET_HEAD_MASK_ZERO_H);}

USE_PROTOCOL_NAMESPACE;

extern OptionUtil g_opt;


TcpAcceptUser::TcpAcceptUser( SOCKET socket, ITcpUserNotify* notify) :
socket_(socket),
notify_(notify),
data_len_( 0)
{
}

TcpAcceptUser::~TcpAcceptUser(void)
{
	close();
}

void TcpAcceptUser::close()
{
	if (  INVALID_SOCKET != socket_)
	{
		closesocket( socket_);
		socket_ = INVALID_SOCKET;
	}

	notify_ = 0;
	data_len_ = 0;

	NS_STL::list<BasicProtocol *>::iterator it = send_pro_pools_.begin();
	while ( it != send_pro_pools_.end())
	{
		BasicProtocol* p = *it;
		delete p;
		it = send_pro_pools_.erase(it);
	}
}

void TcpAcceptUser::do_main( DWORD now)
{
	if (!recv_protocol())
	{
		if (notify_)
			notify_->onClosed(socket_);
		return;
	}

	if ( !send_protocol())
	{
		if ( notify_)
			notify_->onClosed( socket_);
		return;
	}
}

void TcpAcceptUser::send_protocol( BasicProtocol* p)
{
	send_pro_pools_.push_back( p);
}

bool TcpAcceptUser::send_protocol()
{
	NS_STL::list<BasicProtocol *>::iterator it = send_pro_pools_.begin();
	if ( it != send_pro_pools_.end())
	{
		BasicProtocol* p = *it;

		char buf[SINGLE_PACK_LEN] = { 0 };
		char *pbuf = buf + sizeof(S_UINT_32);

		S_INT_32 len = (S_INT_32)p->ByteSizeLong();
		S_UINT_16 proiid = ProtocolFactory::instance()->proto_to_iid(p);

		p->SerializeToArray(pbuf, len);

		S_UINT_32 pro_head = 0;
		MAKE_NETHEAD(pro_head, proiid, len);

		*((S_UINT_32*)buf) = pro_head;

		int totlen = len + sizeof(S_UINT_32);
		int len2 = 0;
		pbuf = buf;

		while ((len2 = ::send(socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
		{
			totlen -= len2;
			if (totlen <= 0)
				break;
			pbuf = pbuf + len2;
		}

		if (len2 == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				::closesocket(socket_);

				return false;
			}
		}

		//delete protocol
		delete p;
		send_pro_pools_.erase(it);
	}

	return true;
}

bool TcpAcceptUser::recv_protocol()
{
	if (socket_ == INVALID_SOCKET)
		return false;

	char *pbuf = recv_buffer_ + data_len_;
	int len = MAX_PACK_LEN - data_len_;

	int rlen = ::recv(socket_, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(socket_);
			data_len_ = 0;

			return false;
		}

		if (data_len_ < sizeof(S_UINT_32))
			return true;
	}

	if (rlen >= 0)
		data_len_ += rlen;

	//分析现有数据
	if (data_len_ < sizeof(S_INT_32))
		return true;

	pbuf = recv_buffer_;

	S_UINT_32 hd = *((S_UINT_32*)pbuf);

	S_UINT_16 proid = 0;
	S_UINT_16 len2 = 0;
	GET_NETHEAD(hd, proid, len2);

	if (len2 > data_len_ - sizeof(S_UINT_32))
		return true;

	pbuf += sizeof(S_UINT_32);

	BasicProtocol *ret = ProtocolFactory::instance()->iid_to_proto(proid, pbuf, len2);

	//移动内存
	data_len_ -= sizeof(S_UINT_32);
	data_len_ -= len2;
	pbuf += len2;
	memmove(&(recv_buffer_[0]), pbuf, data_len_);

	if (notify_)
		notify_->onRecvProtocol( proid, ret, socket_);

	return true;
}
