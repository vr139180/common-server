#ifndef __NETACCEPTOR_H__
#define __NETACCEPTOR_H__

#include <cmsLib/GlobalSettings.h>
#include <boost/asio.hpp>

#include "cmsLib/net/NetAcceptorEvent.h"
#include "cmsLib/net/NetSession.h"

class NetAcceptor
{
public:
	typedef boost::shared_ptr< boost::asio::ip::tcp::socket >      SocketPtr_t;
	typedef boost::shared_ptr< NetSession >		NetSessionPtr;

public:
	NetAcceptor( NetAcceptorEvent& cb);
	virtual ~NetAcceptor();

	bool begin_listen( const char* ip, int port, int maxnums);
	void end_listen();

	int get_realport() { return this->port_; }

public:

	void handle_refuse( const boost::system::error_code& error);
	void handle_accept( NetSessionPtr session, const boost::system::error_code& error);

protected:

	void try_accept();

protected:
	bool				is_initialied_;

	NetAcceptorEvent&	event_cb_;

	//when connection reach limit number, use to fill aync_accept function
	SocketPtr_t			refuse_socket_;
	int					port_;

	boost::asio::ip::tcp::acceptor*	acceptor_;

	int					max_sessions_;
};

#endif	//__NETACCEPTOR_H__
