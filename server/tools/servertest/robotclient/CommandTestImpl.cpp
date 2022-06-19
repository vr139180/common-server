#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

#define NET_HEAD_MASK_ZERO_H	0x0000FFFF
#define NET_HEAD_MASK_ZERO_L	0xFFFF0000

#define MAKE_NETHEAD( VAL, PROID, LENS) { VAL = LENS; VAL = (VAL << 16)&NET_HEAD_MASK_ZERO_L; VAL = VAL|(PROID&NET_HEAD_MASK_ZERO_H);}
#define GET_NETHEAD( VAL, PROID, LENS) { LENS = (S_UINT_16)((VAL&NET_HEAD_MASK_ZERO_L)>>16); PROID = (S_UINT_16)(VAL&NET_HEAD_MASK_ZERO_H);}


USE_PROTOCOL_NAMESPACE;

CommandTestImpl::CommandTestImpl():
hthd( 0),
dwThdId( 0),
parent_wnd_( 0),
data_len_( 0),
gts_data_len_( 0),
socket_( INVALID_SOCKET),
socket2_( INVALID_SOCKET),
thread_step_( 0)
{
	data_ = ProtocolFactory::instance();
	data_->init_factory();
	context_.init_scriptcontext();

	context_.regist_2_context<CommandTestImpl>( "cmd", this);

	//init
	userid_ =NO_INITVALUE;
	role_iid_ =NO_INITVALUE;
}

CommandTestImpl::~CommandTestImpl(void)
{
    stopThread();

	if( socket_ != INVALID_SOCKET)
	{
		::closesocket( socket_);
		socket_ =INVALID_SOCKET;
	}

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

bool CommandTestImpl::connect_to_lgs()
{
	socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket_ == INVALID_SOCKET)
		return false;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( lgs_ip_.c_str());
	serv_addr.sin_port			= htons( lgs_port_);

	if( ::connect( socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		::closesocket( socket_);
		socket_ =INVALID_SOCKET;
		return false;
	}
	else
	{        
		u_long ll =1;
		ioctlsocket( socket_, FIONBIO, &ll);
		ll =1024*8;
		ioctlsocket( socket_, FIONREAD, &ll);        
		return true;
	}
}

bool CommandTestImpl::send_to_lgs( BasicProtocol *p)
{
	if( socket_ == INVALID_SOCKET)
		return false;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof(S_UINT_32);

	S_INT_32 len = (S_INT_32)p->ByteSizeLong();
	S_UINT_16 proiid = data_->proto_to_iid(p);

	p->SerializeToArray(pbuf, len);

	S_UINT_32 pro_head = 0;
	MAKE_NETHEAD(pro_head, proiid, len);

	*((S_UINT_32*)buf) = pro_head;

	int totlen =len + sizeof(S_UINT_32);
	int len2 =0;
	pbuf =buf;

	while( ( len2 =::send( socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		int err =WSAGetLastError();
		if( err != WSAEWOULDBLOCK)
		{
			::closesocket( socket_);
			socket_ =INVALID_SOCKET;
			//reset recvbuf
			data_len_ =0;

			return false;
		}
	}
	return true;
}

BasicProtocol* CommandTestImpl::recv_from_lgs(S_UINT_16& proid)
{
	if( socket_ == INVALID_SOCKET)
		return 0;

	char *pbuf = recv_buffer_ + data_len_;
	int len = MAX_PACK_LEN - data_len_;

	int rlen = ::recv(socket_, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(socket_);
			socket_ = INVALID_SOCKET;
			//reset recvbuf
			data_len_ = 0;

			CString *pstr = new CString();
			pstr->Format("和lgs的socket连接断开\r\n");
			PostMessage(parent_wnd_, WM_USER + 0x200, (WPARAM)pstr, 0);

			return 0;
		}

		if (data_len_ < sizeof(S_UINT_32))
			return 0;
	}

	if (rlen >= 0)
		data_len_ += rlen;

	//分析现有数据
	{
		if (data_len_ < sizeof(S_UINT_32))
			return 0;
		pbuf = recv_buffer_;

		S_UINT_32 hd = *((S_UINT_32*)pbuf);

		S_UINT_16 len2 = 0;
		proid = 0;
		GET_NETHEAD(hd, proid, len2);

		if (len2 > data_len_ - sizeof(S_UINT_32))
			return 0;

		pbuf += sizeof(S_UINT_32);

		BasicProtocol *ret = data_->iid_to_proto(proid, pbuf, len2);
		
		//移动内存
		data_len_ -= sizeof(S_UINT_32);
		data_len_ -= len2;
		pbuf += len2;
		memmove(&(recv_buffer_[0]), pbuf, data_len_);

		return ret;
	}
}

void CommandTestImpl::disconnect_to_lgs()
{
	if( socket_ == INVALID_SOCKET)
		return;

	::closesocket( socket_);
	socket_ =INVALID_SOCKET;
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

	char buf[SINGLE_PACK_LEN] = { 0 };
	char *pbuf = buf + sizeof(S_UINT_32);

	S_INT_32 len = (S_INT_32)p->ByteSizeLong();
	S_UINT_16 proiid = data_->proto_to_iid(p);

	p->SerializeToArray(pbuf, len);

	S_UINT_32 pro_head = 0;
	MAKE_NETHEAD(pro_head, proiid, len);

	*((S_UINT_32*)buf) = pro_head;

	int totlen = len + sizeof(S_UINT_32);
	int len2 = 0;
	pbuf = buf;

	while( ( len2 =::send( socket2_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
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

		if (gts_data_len_ < sizeof(S_UINT_32))
			return 0;
	}

	if (rlen >= 0)
		gts_data_len_ += rlen;

	//分析现有数据
	{
		if (gts_data_len_ < sizeof(S_INT_32))
			return 0;

		pbuf = gts_recv_buffer_;

		S_UINT_32 hd = *((S_UINT_32*)pbuf);

		proid = 0;
		S_UINT_16 len2 = 0;
		GET_NETHEAD(hd, proid, len2);

		if (len2 > gts_data_len_ - sizeof(S_UINT_32))
			return 0;

		pbuf += sizeof(S_UINT_32);

		BasicProtocol *ret = data_->iid_to_proto(proid, pbuf, len2);

		//移动内存
		gts_data_len_ -= sizeof(S_UINT_32);
		gts_data_len_ -= len2;
		pbuf += len2;
		memmove(&(gts_recv_buffer_[0]), pbuf, gts_data_len_);

		return ret;
	}
}

bool CommandTestImpl::connect_to_gts()
{
	socket2_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket2_ == INVALID_SOCKET)
		return false;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( gts_ip_.c_str());
	serv_addr.sin_port			= htons( gts_port_);

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

		if( pthis->thread_step_ & 1)
			pthis->lgs_linkdo();
		if( pthis->thread_step_ & 2)
			pthis->gts_linkdo();
	}

	return 1;
}

std::string CommandTestImpl::get_status()
{
	std::string ret ="";
	if( userid_ == NO_INITVALUE)
	{
		ret ="未登陆\r\n";
		return ret;
	}

	CString fm;
	fm.Format( "用户[%s / %d] role_iid_[%d] 已经登陆\r\n", username_.c_str(), userid_, role_iid_);
	ret =(LPCTSTR)fm;

	return ret;
}

bool CommandTestImpl::islogon()
{
	return userid_ != NO_INITVALUE;
}

bool CommandTestImpl::isplayersel()
{
	return role_iid_ != NO_INITVALUE;
}
