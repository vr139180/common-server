#ifndef __NETCONNECTOR_H__
#define __NETCONNECTOR_H__

#include <cmsLib/GlobalSettings.h>

#include <boost/asio.hpp>

#include "cmsLib/net/NetSession.h"

class NetConnector
{
public:
	typedef boost::shared_ptr< boost::asio::ip::tcp::socket >		SocketPtr_t;

public:
	NetConnector();
	virtual ~NetConnector();

	void set_netsession( NetSession* ns){ net_session_ =ns; }

	bool connect_to( const char* ip, int port);

	bool is_connecting(){ return is_connecting_;}

	void force_close();

public:
	
	void handle_connect( boost::system::error_code error);

private:
	bool			is_connecting_;

	NetSession*		net_session_;
};

#endif	//__NETCONNECTOR_H__
