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

#include "cmsLib/net/NetConnector.h"
#include "cmsLib/net/NetDriverX.h"

NetConnector::NetConnector():
	is_connecting_(false),
	net_session_(0)
{

}

NetConnector::~NetConnector()
{

}

bool NetConnector::connect_to( const char* ip, int port)
{
	if( is_connecting_)
		return false;

	this->ip_ = ip;
	this->port_ = port;

	net_session_->attachToSocket( NetDriverX::getInstance().get_ioservice());

	boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port);

	boost::system::error_code err;
	endpoint.address( boost::asio::ip::address_v4::from_string( ip, err));

	if (err)
	{
		net_session_->on_connectto_result( false);
		return false;
	}

	is_connecting_ = true;

	//timeout 30 s
	if (net_session_->get_session_type() == NetSessionType::NSType_WebSocket)
	{
		boost::beast::get_lowest_layer( net_session_->get_websocket()).expires_after(std::chrono::seconds(30));

		boost::beast::get_lowest_layer(net_session_->get_websocket()).async_connect(endpoint,
			boost::bind(&NetConnector::handle_connect_websocket, this, boost::placeholders::_1));
	}
	else
	{
		net_session_->get_tcpsocket().async_connect(endpoint,
			boost::bind(&NetConnector::handle_connect, this, boost::placeholders::_1));
	}

	return true;
}

void NetConnector::handle_connect( boost::system::error_code error)
{
	net_session_->on_connectto_result(!error);

	is_connecting_ = false;
}

void NetConnector::handle_connect_websocket(boost::beast::error_code error)
{
	net_session_->on_connectto_result_ws(!error, ip_.c_str(), port_);

	is_connecting_ = false;
}

void NetConnector::force_close()
{
	is_connecting_ =false;
}
