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

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/Log.h>

USE_PROTOCOL_NAMESPACE;

CommandTestImpl::CommandTestImpl():
hthd( 0),
dwThdId( 0),
parent_wnd_( 0),
gts_data_len_( 0),
socket2_( INVALID_SOCKET)
{
	data_ = ProtocolFactory::instance();
	data_->init_factory();
	context_.init_scriptcontext();

	lua_State* l = context_.get_luastate();
	this->InitScriptBind(l);

	context_.regist_2_context<CommandTestImpl>( "cmd", this);

	//init
	user_iid_ = NO_INITVALUE;
	user_token_ = NO_INITVALUE;
	role_iid_ = NO_INITVALUE;

	head_.encryption_ = false;
	head_.version_ = 1;
	head_.channel_ = 1;
}

CommandTestImpl::~CommandTestImpl(void)
{
    stopThread();

	if( socket2_ != INVALID_SOCKET)
	{
		::closesocket( socket2_);
		socket2_ =INVALID_SOCKET;
	}

	context_.uninit_scriptcontext();
	data_->uinit();
}

int CommandTestImpl::RunLuaShellCmd( std::string& cmd)
{
	return context_.run_script( cmd.c_str());
}

bool CommandTestImpl::IsScriptInit()
{
	return context_.get_luastate() != 0;
}

void CommandTestImpl::startThread()
{
	if( hthd)
		return;

	hthd=CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)Thread, this,0,&dwThdId);
}

void CommandTestImpl::stopThread()
{
	if( hthd > 0)
	{
		TerminateThread( hthd, dwThdId);
		CloseHandle( hthd);
	}

	hthd =0;
	dwThdId =0;
}

void CommandTestImpl::disconnect_to_gts()
{
	if( socket2_ == INVALID_SOCKET)
		return;

	::closesocket( socket2_);
	socket2_ =INVALID_SOCKET;
}

bool CommandTestImpl::send_to_gts( BasicProtocol *p)
{
	if( socket2_ == INVALID_SOCKET)
		return false;

	//协议号递增
	++head_.seqno_;

	char buf[SINGLE_PACK_LEN] = { 0 };
	S_UINT_8 *pbuf = (S_UINT_8*)buf;
	S_UINT_8 *pdata = (S_UINT_8*)pbuf;

	S_UINT_16 proiid = data_->proto_to_iid(p);
	if (proiid == 0)
		return false;
	//设置协议id
	head_.set_msgid(proiid);

	if (!head_.encode_head(pdata, SINGLE_PACK_LEN))
		return false;

	S_UINT_32 len2 = (S_UINT_32)p->ByteSizeLong();
	if (head_.get_headlen() + len2 > SINGLE_PACK_LEN)
		return false;

	try {
		pdata = pbuf + head_.get_headlen();
		if (!p->SerializeToArray(pdata, len2))
			return false;
	}
	catch (...) {
		return false;
	}

	head_.encode_totlelen(pbuf, SINGLE_PACK_LEN, len2);
	int totlen = head_.get_totlelen();

	pdata = pbuf;
	while( ( len2 =::send( socket2_, (const char*)pdata, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pdata = pdata + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket( socket2_);
			socket2_ =INVALID_SOCKET;
			//reset recvbuf
			gts_data_len_ =0;

			return false;
		}
	}

	return true;
}

BasicProtocol* CommandTestImpl::recv_from_gts(S_UINT_16& proid)
{
	if( socket2_ == INVALID_SOCKET)
		return 0;

	char *pbuf = gts_recv_buffer_ + gts_data_len_;
	int len = MAX_PACK_LEN - gts_data_len_;

	int rlen = ::recv(socket2_, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(socket2_);
			socket2_ = INVALID_SOCKET;
			gts_data_len_ = 0;

			CString *pstr = new CString();
			pstr->Format("和gts的socket连接断开\r\n");
			PostMessage(parent_wnd_, WM_USER + 0x200, (WPARAM)pstr, 0);

			return 0;
		}
	}

	if (rlen >= 0)
		gts_data_len_ += rlen;

	if (gts_data_len_ < sizeof(S_UINT_32))
		return 0;

	//分析现有数据
	S_UINT_8* pdata = (S_UINT_8*)gts_recv_buffer_;
	S_UINT_32 prolen = *((S_UINT_32*)pdata);
	{
		S_UINT_32 offset = 0;
		CProtoHeadBase::Decode(pdata, (S_UINT_32)gts_data_len_, offset, prolen);
	}

	if (gts_data_len_ < prolen)
		return 0;

	CProtocolHead chead;
	if (!chead.decode_head(pdata, prolen))
		return 0;

	pdata = (S_UINT_8*)gts_recv_buffer_ + (S_UINT_32)chead.get_headlen();
	try {
		BasicProtocol* bp = data_->iid_to_proto(chead.get_msgid(), pdata, chead.get_msglen());
		if (bp == 0)
			return 0;

		//移动内存
		gts_data_len_ -= prolen;
		if (gts_data_len_ > 0)
		{
			pdata = (S_UINT_8*)gts_recv_buffer_ + prolen;
			memmove(&(gts_recv_buffer_[0]), pdata, gts_data_len_);
		}

		return bp;
	}
	catch (...) {
		return 0;
	}
}

bool CommandTestImpl::connect_to_gts()
{
	socket2_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket2_ == INVALID_SOCKET)
		return false;

	std::string ip;
	int port = 0;
	svrinfo_->get_ipinfo(ip, port);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( ip.c_str());
	serv_addr.sin_port			= htons( port);

	if( ::connect( socket2_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		::closesocket( socket2_);
		socket2_ =INVALID_SOCKET;
		return false;
	}
	else
	{        
		u_long ll =1;
		ioctlsocket( socket2_, FIONBIO, &ll);
		ll =1024*8;
		ioctlsocket( socket2_, FIONREAD, &ll);        
		return true;
	}
}

DWORD  WINAPI CommandTestImpl::Thread(LPVOID  lparam)
{
	CommandTestImpl *pthis =reinterpret_cast<CommandTestImpl*>(lparam);
	while( 1)
	{
		::Sleep( 1);

		pthis->gts_linkdo();
	}

	return 1;
}

std::string CommandTestImpl::get_status()
{
	std::string ret ="";
	if( user_iid_ == NO_INITVALUE)
	{
		ret ="未登陆\r\n";
		return ret;
	}

	CString fm;
	fm.Format( "用户[%s / %d] role_iid_[%d] 已经登陆\r\n", username_.c_str(), user_iid_, role_iid_);
	ret =(LPCTSTR)fm;

	return ret;
}

bool CommandTestImpl::islogon()
{
	return user_iid_ != NO_INITVALUE;
}

bool CommandTestImpl::isplayersel()
{
	return role_iid_ != NO_INITVALUE;
}
