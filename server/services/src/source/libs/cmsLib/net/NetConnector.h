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
