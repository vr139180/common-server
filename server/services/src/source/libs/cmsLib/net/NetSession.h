#ifndef __NETSESSION_H__
#define __NETSESSION_H__

#include <cmsLib/GlobalSettings.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/bind/bind.hpp>

#include <google/protobuf/message.h>

#include <cmsLib/prolib/ProtocolFactory.h>
#include <cmsLib/net/NetSessionBindEvent.h>
#include <cmsLib/ThreadLock.h>

#include <deque>

#define NET_SINGLE_PACKAGE_SIZE_MAX		16*1024	//max package size for single protocol 16k
#define NET_RECVBUFF_SIZE_MAX			NET_SINGLE_PACKAGE_SIZE_MAX*2	//double single package
#define NET_SENDBUFF_SIZE_MAX			NET_SINGLE_PACKAGE_SIZE_MAX*2	//double single package

class NetSession
{
	friend class NetAcceptor;
	friend class NetConnector;
public:
	typedef std::deque<BasicProtocol*>				BasicProtocolQueue_t;
	typedef std::deque<BasicProtocol*>::iterator	BasicProtocolQueueIt_t;

	typedef boost::shared_ptr< boost::asio::ip::tcp::socket >	SocketPtr_t;

public:
	explicit NetSession( ThreadLock* lock, ThreadLock* queuelock, NetSessionBindEvent* cb =0);
	virtual ~NetSession();

	void force_reset();

	void send_protocol( BasicProtocol* message);

	void heart_beat();
	
	std::string get_ip();

	bool is_socket_valid(){
		return socket_ != 0;
	}

public:

	void handle_write( boost::system::error_code error, size_t bytes_transferred);
	void handle_read( boost::system::error_code error, size_t bytes_transferred);

protected:
	
	void attachToSocket( boost::asio::io_service& ios);
	boost::asio::ip::tcp::socket& get_socket(){ return *socket_;};

	void on_connectto_result(bool success);
	void on_connectfrom_result(bool success);

	bool try_read_nomutext( int alreadyrecvsize, 
		std::list<S_UINT_16>& readproiids, std::list<BasicProtocol*>& readpro);
	void try_write_nomutext( int alreadysendsize);

	void fill_writebuffer_nomutex( int alreadysendsize);
	int analy_package_nomutex(std::list<S_UINT_16>& readproiids, std::list<BasicProtocol*>& readpro);

	void on_connect_lost_netthread();

	void reset_nomutex();
	void freeSendQueue();

	void dealwith_connect_result(bool success);

private:
	volatile bool				is_initialized;

	NetSessionBindEvent*		bind_event_cb_;
	//link to socket
	SocketPtr_t                 socket_;

	boost::asio::io_service*    io_service_;

	//waiting to send messages queue
	BasicProtocolQueue_t        send_queue_;
	ThreadLock*                 lock_;
	ThreadLock*					queue_lock_;
	bool						lock_delete_;

	ProtocolFactory*            pro_factory_;

	c8				recv_buff_[NET_RECVBUFF_SIZE_MAX];
	volatile  int	recv_buff_pos_;

	c8				send_buff_[NET_SENDBUFF_SIZE_MAX];
	volatile  int	send_buff_pos_;
	volatile  bool	is_sending_;
};

#endif	//__NETSESSION_H__
