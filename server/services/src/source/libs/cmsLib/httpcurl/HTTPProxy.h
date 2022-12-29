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
#ifndef __HTTP_PROXY_H__
#define __HTTP_PROXY_H__

#include <string>

class HTTPProxy
{
public:
	HTTPProxy(): m_host(""), m_port(0), m_username(""), m_password(""){}

	~HTTPProxy(){}

	HTTPProxy(const std::string& host, unsigned int port,
		const std::string& username = "", const std::string& password = "") 
		: m_host(host)
		, m_port(port)
		, m_username(username)
		, m_password(password)
	{
	}

	const std::string &GetProxyName() const
	{
		return m_host;
	}

	void SetProxyName(const std::string& host)
	{
		m_host = host;
	}

	unsigned int GetProxyPort() const
	{
		return m_port;
	}

	void SetProxyPort( unsigned int port)
	{
		m_port = port;
	}

	const std::string &GetUsername() const
	{
		return m_username;
	}

	void SetUsername(const std::string& username)
	{
		m_username = username;
	}

	const std::string &GetPassword() const
	{
		return m_password;
	}

	void SetPassword(const std::string& password)
	{
		m_password = password;
	}

private:
	std::string		m_host;
	unsigned int	m_port;
	std::string		m_username;
	std::string		m_password;
};

#endif //__HTTP_PROXY_H__