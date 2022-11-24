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

#ifndef __NETSESSION_H__
#define __NETSESSION_H__

#include <google/protobuf/message.h>

#include <cmsLib/GlobalSettings.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/bind/bind.hpp>

#include <cmsLib/net/NetSessionBindEvent.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/net/ws_custom_static_buffer.h>

#include <deque>

#define NET_SINGLE_PACKAGE_SIZE_MAX		16*1024	//max package size for single protocol 16k
#define NET_RECVBUFF_SIZE_MAX			NET_SINGLE_PACKAGE_SIZE_MAX*2	//double single package
#define NET_SENDBUFF_SIZE_MAX			NET_SINGLE_PACKAGE_SIZE_MAX*2	//double single package


class NetSession
{
	friend class NetAcceptor;
	friend class NetConnector;
public:
	typedef std::deque<NetProtocol*>				NetProtocolQueue_t;
	typedef std::deque<NetProtocol*>::iterator		NetProtocolQueueIt_t;

	typedef boost::shared_ptr<boost::asio::ip::tcp::socket>				SocketPtr_t;
	typedef boost::beast::websocket::stream<boost::beast::tcp_stream>	WebSocketStream;
	typedef boost::shared_ptr<WebSocketStream>							WebSocketPtr_t;

public:
	explicit NetSession( ThreadLock* lock, ThreadLock* queuelock, 
		NetSessionBindEvent* cb =0, NetSessionType nstype = NetSessionType::NSType_TCP);

	virtual ~NetSession();

	void force_reset();

	void send_protocol( NetProtocol* message);

	void heart_beat();
	
	std::string get_ip();

	bool is_socket_valid();

	NetSessionType get_session_type() { return socket_type_; }

	NetSessionBindEvent* get_bindevent() { return bind_event_cb_; }

public:

	void handle_write( boost::system::error_code error, size_t bytes_transferred);
	void handle_read( boost::system::error_code error, size_t bytes_transferred);

	void on_accept_websocket(boost::beast::error_code ec);

protected:
	
	void attachToSocket( boost::asio::io_service& ios);
	boost::asio::ip::tcp::socket& get_socket();
	boost::asio::ip::tcp::socket& get_tcpsocket();
	WebSocketStream& get_websocket();

	void on_connectto_result_ws(bool success, const char* ip, int port);
	void on_handshake_ws(boost::beast::error_code ec);

	void on_connectto_result(bool success);
	void on_connectfrom_result(bool success);

	bool try_read_nomutext( int alreadyrecvsize, std::list<NetProtocol*>& readpro);
	void try_write_nomutext( int alreadysendsize);

	void fill_writebuffer_nomutex( int alreadysendsize);
	int analy_package_nomutex(std::list<NetProtocol*>& readpro);

	void on_connect_lost_netthread();

	void reset_nomutex();
	void freeSendQueue();

	void dealwith_connect_result(bool success);
	void dealwith_connect_tows_result(bool success);

private:
	volatile bool				is_initialized;

	NetSessionBindEvent*		bind_event_cb_;
	//link to socket
	SocketPtr_t					socket_;
	NetSessionType				socket_type_;
	WebSocketPtr_t				websocket_;

	boost::asio::io_service*    io_service_;

	//waiting to send messages queue
	NetProtocolQueue_t			send_queue_;
	ThreadLock*                 lock_;
	ThreadLock*					queue_lock_;
	bool						lock_delete_;

	S_UINT_8		recv_buff_[NET_RECVBUFF_SIZE_MAX];
	volatile  int	recv_buff_pos_;
	//ws_custom_static_buffer< NET_RECVBUFF_SIZE_MAX>		recv_buff_ws_;
	boost::beast::flat_static_buffer<NET_SINGLE_PACKAGE_SIZE_MAX>	recv_buff_ws_;

	S_UINT_8		send_buff_[NET_SENDBUFF_SIZE_MAX];
	volatile  int	send_buff_pos_;
	volatile  bool	is_sending_;
};

#endif	//__NETSESSION_H__
