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

#define NET_HEAD_MASK_ZERO_H	0x0000FFFF
#define NET_HEAD_MASK_ZERO_L	0xFFFF0000

#define MAKE_NETHEAD( VAL, PROID, LENS) { VAL = LENS; VAL = (VAL << 16)&NET_HEAD_MASK_ZERO_L; VAL = VAL|(PROID&NET_HEAD_MASK_ZERO_H);}
#define GET_NETHEAD( VAL, PROID, LENS) { LENS = (S_UINT_16)((VAL&NET_HEAD_MASK_ZERO_L)>>16); PROID = (S_UINT_16)(VAL&NET_HEAD_MASK_ZERO_H);}


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
lgs_data_len_( 0),
lgs_socket_( INVALID_SOCKET),
gts_data_len_( 0),
gts_socket_( INVALID_SOCKET),
cancel_( false)
{
	need_connect_2_lgs_ =false;
	need_connect_2_gts_ = false;
	is_connect_2_gts_failed_ =false;
	is_connect_2_lgs_failed_ = false;
	delay_connect_ =0;

	workflow_.init( this, &vuser_);

	data_ = ProtocolFactory::instance();

	cur_user_pos_ =0;
}

void TestCase::set_userrange( int id, int range, int svrno)
{
	userid_ =id;
	userrange_ =range;
	cur_user_pos_ =-1;
	svrno_ =svrno;
}

void TestCase::set_userid()
{
	++cur_user_pos_;
	if( cur_user_pos_ >= userrange_)
		cur_user_pos_ =0;

	std::stringstream ss;
	ss << get_userid();
	vuser_.user_name_ =ss.str();
	vuser_.user_pwd_ =ss.str();
}

int TestCase::get_userid()
{
	return userid_ + cur_user_pos_;
}

TestCase::~TestCase(void)
{
}

void TestCase::run_it( ULONGLONG now)
{
	if( cancel_)
		return;

	if( need_connect_2_lgs_)
	{
		if( start_connect_t_ + delay_connect_ > now)
			return;

		need_connect_2_lgs_ =false;

		int err =0;
		if( !connect_2_lgs( err))
		{
			is_connect_2_lgs_failed_ = true;
			logDebug(out_runtime, "--------------------------------------- connect to lgs failed err:%d", err);
			return;
		}

		need_connect_2_lgs_ =false;
	}

	if( need_connect_2_gts_)
	{
		if( start_connect_t_ + delay_connect_ > now)
			return;

		need_connect_2_gts_ =false;

		int err =0;
		if( !connect_2_gts( err))
		{
			is_connect_2_gts_failed_ =true;
			logDebug(out_runtime, "--------------------------------------- connect to gts failed err:%d", err);
			return;
		}

	}

	workflow_.update( now);

	recv_lgs_protocol();

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

void TestCase::send_lgs_protocol( BasicProtocol *p)
{
	if( lgs_socket_ == INVALID_SOCKET)
		return;

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

	while ((len2 = ::send(lgs_socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
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
			::closesocket(lgs_socket_);
			lgs_socket_ = INVALID_SOCKET;
			//reset recvbuf
			lgs_data_len_ = 0;
		}
	}
}

void TestCase::recv_lgs_protocol()
{
	if( lgs_socket_ == INVALID_SOCKET)
		return;

	char *pbuf = lgs_recv_buffer_ + lgs_data_len_;
	int len = MAX_PACK_LEN - lgs_data_len_;

	int rlen = ::recv(lgs_socket_, pbuf, len, 0);

	if (rlen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket(lgs_socket_);
			lgs_socket_ = INVALID_SOCKET;
			//reset recvbuf
			lgs_data_len_ = 0;

			return;
		}

		if (lgs_data_len_ < sizeof(S_UINT_32))
			return;
	}

	if (rlen >= 0)
		lgs_data_len_ += rlen;

	//分析现有数据
	if (lgs_data_len_ < sizeof(S_UINT_32))
		return;
	pbuf = lgs_recv_buffer_;

	S_UINT_32 hd = *((S_UINT_32*)pbuf);

	S_UINT_16 len2 = 0;
	S_UINT_16 proid = 0;
	GET_NETHEAD(hd, proid, len2);

	if (len2 > lgs_data_len_ - sizeof(S_UINT_32))
		return;

	pbuf += sizeof(S_UINT_32);

	BasicProtocol *ret = data_->iid_to_proto(proid, pbuf, len2);

	//移动内存
	lgs_data_len_ -= sizeof(S_UINT_32);
	lgs_data_len_ -= len2;
	pbuf += len2;
	memmove(&(lgs_recv_buffer_[0]), pbuf, lgs_data_len_);

	if (ret)
	{
		workflow_.recv_protocol(proid, ret);
		delete ret;
	}
}

bool TestCase::connect_2_lgs( int& err)
{
	set_userid();

	if( lgs_socket_ != INVALID_SOCKET)
		disconnect_2_lgs();

	lgs_socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( lgs_socket_ == INVALID_SOCKET)
		return false;

	int one =1;
	::setsockopt( lgs_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof( one));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( (LPCTSTR)lgs_ip_.c_str());
	serv_addr.sin_port			= htons( lgs_port_);

	if( ::connect( lgs_socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		err =WSAGetLastError();
		::closesocket( lgs_socket_);
		lgs_socket_ =INVALID_SOCKET;

		return false;
	}
	else
	{
		u_long ll =1;
		ioctlsocket( lgs_socket_, FIONBIO, &ll);

		return true;
	}
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
	MAKE_NETHEAD(pro_head, proiid, len);

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
	GET_NETHEAD(hd, proid, len2);

	if (len2 > gts_data_len_ - sizeof(S_UINT_32))
		return;

	pbuf += sizeof(S_UINT_32);

	BasicProtocol *ret = data_->iid_to_proto(proid, pbuf, len2);

	//移动内存
	gts_data_len_ -= sizeof(S_UINT_32);
	gts_data_len_ -= len2;
	pbuf += len2;
	memmove(&(gts_recv_buffer_[0]), pbuf, gts_data_len_);

	if (ret)
	{
		workflow_.recv_protocol(proid, ret);
		delete ret;
	}

}

void TestCase::disconnect_2_lgs()
{
	if( lgs_socket_ != INVALID_SOCKET)
	{
		::closesocket( lgs_socket_);

		lgs_socket_ =INVALID_SOCKET;
	}
}

void TestCase::reconnect_2_lgs()
{
	need_connect_2_lgs_ =true;
	is_connect_2_lgs_failed_ = false;
	start_connect_t_ =TestCaseWorkFlow::GetMillisecondTime();
}

void TestCase::reconnect_2_gts()
{
	need_connect_2_gts_ =true;
	is_connect_2_gts_failed_ =false;
	start_connect_t_ =TestCaseWorkFlow::GetMillisecondTime();
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
