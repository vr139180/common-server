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

#include "windows.h"

#include <cmsLib/Log.h>
#include <cmsLib/util/ShareUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include "testcase.h"
#include "MsgNotify.h"

extern CWindow*		g_mainwnd;

#define WM_INFONOTIFY  WM_USER+0x100
#define WM_ERRORNOTIFY  WM_USER+0x101

void post_msg( MsgNotify *msg)
{
	if( g_mainwnd == 0)
	{
		delete msg;
		return;
	}

	g_mainwnd->PostMessage( WM_INFONOTIFY, (WPARAM)msg, 0);
}

void post_error( MsgNotify *msg)
{
	if( g_mainwnd == 0)
	{
		delete msg;
		return;
	}

	g_mainwnd->PostMessage( WM_ERRORNOTIFY, (WPARAM)msg, 0);
}

TestCase::TestCase()
:data_( 0),
gts_data_len_( 0),
gts_socket_( INVALID_SOCKET),
cancel_( false)
{
	need_connect_2_gts_ = false;
	is_connect_2_gts_failed_ =false;

	workflow_.init( this, &vuser_);
	vuser_.set_testcase(&workflow_);

	data_ = ProtocolFactory::instance();
}

void TestCase::set_userinfo(int id, const char* urladdr, const char* prefix)
{
	userid_ = id;
	vuser_.set_userinfo(id, urladdr, prefix);
}

void TestCase::set_openprefix(const char* prefix)
{
	vuser_.set_openprefix(prefix);
}

int TestCase::get_userid()
{
	return userid_;
}

TestCase::~TestCase(void)
{
}

void TestCase::init_clientproto(int serverid, std::string account)
{
	//sdo_wrap_.init_clientproto(serverid, account);
}

void TestCase::run_it( ULONGLONG now)
{
	if( cancel_)
		return;

	if (need_connect_2_gts_)
	{
		need_connect_2_gts_ = false;

		gts_connect_errcode_ = 0;
		if (!connect_2_gts(gts_connect_errcode_))
		{
			is_connect_2_gts_failed_ = true;
			return;
		}

	}

	workflow_.update(now);
	this->heart_beat(now);

	recv_gts_protocol();
}

extern std::string		g_xml;

void TestCase::start_case( bool notify)
{
	workflow_.prepare( g_xml.c_str());

	if( notify)
	{
		MsgNotify *msg =new MsgNotify();
		msg->userid_ =userid_;
		msg->msg_.Format("user%d start....", userid_);
		post_msg( msg);
	}
}

void TestCase::stop_case( bool notify)
{
	if( notify)
	{
		MsgNotify *msg =new MsgNotify();
		msg->userid_ =userid_;
		msg->msg_.Format("user%d end....", userid_);
		post_msg( msg);
	}

	wait_socket_close();
}

void TestCase::end_case()
{
	MsgNotify *msg = new MsgNotify();
	msg->userid_ = userid_;
	msg->msg_.Format("user%d end....", userid_);
	post_msg(msg);

	wait_socket_close();

	this->cancel_ = true;
}

bool TestCase::connect_2_gts( int& err)
{
	if( gts_socket_ != INVALID_SOCKET)
		disconnect_2_gts();

	gts_socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( gts_socket_ == INVALID_SOCKET)
		return false;

	int one =1;
	::setsockopt( gts_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof( one));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( vuser_.gts_ip_.c_str());
	serv_addr.sin_port			= htons( vuser_.gts_port_);

	if( ::connect( gts_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		err =WSAGetLastError();
		::closesocket( gts_socket_);
		gts_socket_ =INVALID_SOCKET;

		return false;
	}
	else
	{
		u_long ll =1;
		ioctlsocket( gts_socket_, FIONBIO, &ll);

		return true;
	}
}

void TestCase::send_gts_protocol( BasicProtocol *p)
{
	if( gts_socket_ == INVALID_SOCKET)
		return;

	char buf[SINGLE_PACK_LEN] = { 0 };
	char *pbuf = buf + sizeof(S_UINT_32);

	S_INT_32 len = (S_INT_32)p->ByteSizeLong();
	S_UINT_16 proiid = data_->proto_to_iid(p);

	p->SerializeToArray(pbuf, len);

	S_UINT_32 pro_head = 0;

	*((S_UINT_32*)buf) = pro_head;

	int totlen = len + sizeof(S_UINT_32);
	int len2 = 0;
	pbuf = buf;

	while ((len2 = ::send(gts_socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if (totlen <= 0)
			break;
		pbuf = pbuf + len2;
	}

	if (len2 == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK)
		{
			::closesocket(gts_socket_);
			gts_socket_ = INVALID_SOCKET;
			//reset recvbuf
			gts_data_len_ = 0;
		}
	}
}

void TestCase::recv_gts_protocol()
{
	if( gts_socket_ == INVALID_SOCKET){
		return;
	}

	char *pbuf = gts_recv_buffer_ + gts_data_len_;
	int len = MAX_PACK_LEN - gts_data_len_;

	int rlen = ::recv(gts_socket_, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(gts_socket_);
			gts_socket_ = INVALID_SOCKET;
			//reset recvbuf
			gts_data_len_ = 0;

			return;
		}

		if (gts_data_len_ < sizeof(S_UINT_32))
			return;
	}

	if (rlen >= 0)
		gts_data_len_ += rlen;

	//分析现有数据
	if (gts_data_len_ < sizeof(S_UINT_32))
		return;
	pbuf = gts_recv_buffer_;

	S_UINT_32 hd = *((S_UINT_32*)pbuf);

	S_UINT_16 len2 = 0;
	S_UINT_16 proid = 0;

	if (len2 > gts_data_len_ - sizeof(S_UINT_32))
		return;

	pbuf += sizeof(S_UINT_32);

	BasicProtocol *ret = data_->iid_to_proto(proid, pbuf, len2);
	std::auto_ptr<BasicProtocol> ptr(ret);

	//移动内存
	gts_data_len_ -= sizeof(S_UINT_32);
	gts_data_len_ -= len2;
	pbuf += len2;
	memmove(&(gts_recv_buffer_[0]), pbuf, gts_data_len_);

	workflow_.recv_protocol(proid, ret);
}

void TestCase::disconnect_2_gts()
{
	if( gts_socket_ != INVALID_SOCKET)
	{
		::closesocket( gts_socket_);
		gts_socket_ =INVALID_SOCKET;
	}
}

void TestCase::wait_socket_close()
{
	if( gts_socket_ != INVALID_SOCKET)
	{
		::closesocket( gts_socket_);

		gts_socket_ =INVALID_SOCKET;
	}
}

bool TestCase::force_close()
{
	wait_socket_close();

	return true;
}

void TestCase::heart_beat(S_INT_64 tnow)
{
	if (!vuser_.do_heartbeat(tnow))
		return;

	Svr_LiveTick_ntf *req = new Svr_LiveTick_ntf();
	send_gts_protocol(req);
}
