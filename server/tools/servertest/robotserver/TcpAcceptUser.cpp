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

#include <gameLib/protobuf/Proto_all.h>

#include "TcpAcceptUser.h"
#include "OptionUtil.h"

#define MAX_PACK_LEN	16*1024
#define SINGLE_PACK_LEN 1024*16

USE_PROTOCOL_NAMESPACE;

extern OptionUtil g_opt;

TcpAcceptUser::TcpAcceptUser( SOCKET socket, ITcpUserNotify* notify) :
socket_(socket),
notify_(notify),
data_len_( 0)
{
	head_.encryption_ = false;
	head_.version_ = 1;
	head_.channel_ = 1;
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

		//-----------------------------------------------------------------------
		//协议号递增
		++head_.seqno_;

		char buf[ROBOT_SOCKET_BUF] = { 0 };
		S_UINT_8 *pbuf = (S_UINT_8*)buf;
		S_UINT_8 *pdata = (S_UINT_8*)pbuf;

		S_UINT_16 proiid = ProtocolFactory::instance()->proto_to_iid(p);
		if (proiid == 0)
			return false;
		//设置协议id
		head_.set_msgid(proiid);

		if (!head_.encode_head(pdata, ROBOT_SOCKET_BUF))
			return false;

		S_UINT_32 len2 = (S_UINT_32)p->ByteSizeLong();
		if (head_.get_headlen() + len2 > ROBOT_SOCKET_BUF)
			return false;

		try {
			pdata = pbuf + head_.get_headlen();
			if (!p->SerializeToArray(pdata, len2))
				return false;
		}
		catch (...) {
			return false;
		}

		head_.encode_totlelen(pbuf, ROBOT_SOCKET_BUF, len2);
		int totlen = head_.get_totlelen();
		pdata = pbuf;
		//-----------------------------------------------------------------------

		while ((len2 = ::send(socket_, (const char*)pdata, totlen, 0)) != SOCKET_ERROR)
		{
			totlen -= len2;
			if (totlen <= 0)
				break;
			pdata = pdata + len2;
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
	int len = ROBOT_SOCKET_BUF - data_len_;

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
	if (data_len_ < sizeof(S_UINT_32))
		return true;

	//---------------------------------------------------------------
	//分析现有数据
	S_UINT_8* pdata = (S_UINT_8*)recv_buffer_;
	S_UINT_32 prolen = *((S_UINT_32*)pdata);
	{
		S_UINT_32 offset = 0;
		CProtoHeadBase::Decode(pdata, (S_UINT_32)data_len_, offset, prolen);
	}

	if (data_len_ < prolen)
		return true;

	CProtocolHead chead;
	if (!chead.decode_head(pdata, prolen))
		return true;

	pdata = (S_UINT_8*)recv_buffer_ + (S_UINT_32)chead.get_headlen();
	try {
		BasicProtocol* bp = ProtocolFactory::instance()->iid_to_proto(chead.get_msgid(), pdata, chead.get_msglen());
		if (bp == 0)
			return false;

		//移动内存
		data_len_ -= prolen;
		if (data_len_ > 0)
		{
			pdata = (S_UINT_8*)recv_buffer_ + prolen;
			memmove(&(recv_buffer_[0]), pdata, data_len_);
		}

		if (notify_)
			notify_->onRecvProtocol(chead.get_msgid(), bp, socket_);

		return true;
	}
	catch (...) {
		return false;
	}
	//---------------------------------------------------------------
}
