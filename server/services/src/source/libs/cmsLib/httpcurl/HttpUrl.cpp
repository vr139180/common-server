#include <cmsLib/httpcurl/HttpUrl.h>

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <cmsLib/httpcurl/HttpUtilities.h>
#include <cmsLib/util/ShareUtil.h>

namespace {

	bool IsSafe(char c)
	{
		return
			((c >= 'a') && (c <= 'z')) ||
			((c >= 'A') && (c <= 'Z')) ||
			((c >= '0') && (c <= '9'));
	}

	unsigned char CharToHex(char c)
	{
		unsigned char result = 0;
		if ((c >= '0') && (c <= '9'))
		{
			result = c - '0';
		}
		else if ((c >= 'a') && (c <= 'f'))
		{
			result = c - 'a' + 10;
		}
		else if ((c >= 'A') && (c <= 'F'))
		{
			result = c - 'A' + 10;
		}

		return result;
	}
}

HttpUrl::HttpUrl(const std::string& url)
: m_scheme("http")
, m_username("")
, m_password("")
, m_host("localhost")
, m_port(0)
, m_path("")
, m_parameters("")
, m_query("")
, m_fragment("")
{
	ParseURL(url);
}

HttpUrl::~HttpUrl() 
{
}

// Description: Convert HTTPURL to its normal presentation.
const std::string HttpUrl::ConvertToString() const
{
	std::string result;
	result += m_scheme + "://";
	
	if (!m_username.empty())
	{
		result += m_username;
		if (!m_username.empty())
		{
			result += ":" + m_password;
		}
		result += "@";
	}

	result += m_host;
	if (m_port != 0)
	{
		result += ":";
		result += ShareUtil::str_format("%d", m_port).c_str();
	}

	result += "/" + m_path;
	if (!m_parameters.empty())
	{
		result += ";" + m_parameters;
	}

	if (!m_query.empty())
	{
		result += "?" + m_query;
	}

	if (!m_fragment.empty())
	{
		result += "#" + m_fragment;
	}

	return result;
}

const std::string &HttpUrl::GetScheme() const
{
	return m_scheme;
}

const std::string &HttpUrl::GetUsername() const
{
	return m_username;
}

const std::string &HttpUrl::GetPassword() const
{
	return m_password;
}

const std::string &HttpUrl::GetHost() const
{
	return m_host;
}

unsigned int HttpUrl::GetPort() const
{
	return m_port;
}

const std::string &HttpUrl::GetPath() const
{
	return m_path;
}

const std::string &HttpUrl::GetParameters() const
{
	return m_parameters;
}

const std::string &HttpUrl::GetQuery() const
{
	return m_query;
}

const std::string &HttpUrl::GetFragment() const
{
	return m_fragment;
}

// TODO: We should add validator for all those setters.
void HttpUrl::SetScheme(const std::string & scheme)
{
	m_scheme = scheme;
	m_port = 0;
}

void HttpUrl::SetLocation(const std::string & host, unsigned int port)
{
	m_host = host;
	m_port = port;
}

void HttpUrl::SetAuthentification(const std::string & username, const std::string & password)
{
	m_username = username;
	if (!m_username.empty())
	{
		m_password = password;
	}
	else
	{
		m_password = "";
	}
}

void HttpUrl::SetPath(const std::string & path)
{
	m_path = path;
}

void HttpUrl::SetParameters(const std::string &parameters)
{
	m_parameters = parameters;
}

void HttpUrl::SetQuery(const std::string &query)
{
	m_query = query;
}

void HttpUrl::SetFragment(const std::string &fragment)
{
	m_fragment = fragment;
}

const std::string HttpUrl::StripLocation() const {

	std::string result = m_path;
	if (!m_parameters.empty())
	{
		result += ";" + m_parameters;
	}
	
	if (!m_query.empty())
	{
		result += "?" + m_query;
	}
	
	if (!m_fragment.empty())
	{
		result += "#" + m_fragment;
	}

	return result;
}

const std::string HttpUrl::Escape(const std::string& data)
{
	std::vector<char> result;
	char* pData = new char[data.length()+1];

#if defined(EW_PLATFORM_WINDOWS)
	strncpy(pData,data.c_str(),data.length());

#elif defined(EW_PLATFORM_LINUX)
	strncpy(pData,data.c_str(),data.length());

#else
#endif
	//_tcscpy_s(pData,data.length()+1,data.c_str());
	for( unsigned int i = 0 ; i < data.length() ; ++i)
	{
		if (IsSafe(pData[i]))
		{
			result.push_back(pData[i]);
		}
		else
		{
			char buf[4];
			std::string xstr = ShareUtil::str_format("%%%02X", static_cast<char>(pData[i]));
			memcpy(&buf[0], xstr.c_str(), 4);

			result.push_back(buf[0]);
			result.push_back(buf[1]);
			result.push_back(buf[2]);
		}
	}

	result.push_back('\0');
	delete[] pData;
	
	return &result[0];
}

const std::string HttpUrl::UnEscape(const std::string& data)
{
	std::vector<char> result;
	char* pData = new char[data.length()+1];

#if defined(EW_PLATFORM_WINDOWS)
	strncpy(pData,data.c_str(),data.length());

#elif defined(EW_PLATFORM_LINUX)
	strncpy(pData,data.c_str(),data.length());

#else
#endif
	//_tcscpy_s(pData,data.length()+1,data.c_str());

	for( unsigned int i = 0 ; i < data.length() ; ++i)
	{
		if (pData[i] == '%' && (data.length()-i) > 2)
		{
			char b = (CharToHex(pData[i+1]) << 4) + CharToHex(pData[i+2]);
			result.push_back(b);
			i += 2;
		}
		else
		{
			result.push_back(pData[i]);
		}
	}

	result.push_back('\0');
	delete[] pData;
	
	return &result[0];
}

void HttpUrl::ParseURL(const std::string &url)
{
	std::string data(url);
	m_scheme = HttpUtilities::GetNextToken(data,"://");
	if (data.length() == 0)
	{
		m_scheme = "http";
		data = url;
	}

	// Network location handling
	std::string netloc = HttpUtilities::GetNextToken(data,"/");

	std::string userpw = HttpUtilities::GetNextToken(netloc,"@");
	if (!netloc.empty())
	{
		// We have an '@' in the string
		m_username = HttpUtilities::GetNextToken(userpw,":");
		m_password = userpw;
	}
	else
	{
		netloc = userpw;
		m_username = "";
		m_password = "";
	}

	m_host = HttpUtilities::GetNextToken(netloc,":");
	if (!netloc.empty())
	{
		m_port = atoi(netloc.c_str());
	}
	else
	{
		m_port = 0;
	}

	m_path = data;
	int fragStart = m_path.find("#");
	if (fragStart != -1)
	{
		m_fragment = HttpUtilities::TruncateSuffix(m_path, fragStart,1);
	}
	else
	{
		m_fragment = "";
	}
	int queryStart = m_path.find("?");
	if (queryStart != -1)
	{
		m_query = HttpUtilities::TruncateSuffix(m_path, queryStart,1);
	}
	else
	{
		m_query = "";
	}
	int paramsStart = m_path.find(";");
	if (paramsStart != -1)
	{
		m_parameters = HttpUtilities::TruncateSuffix(m_path, paramsStart,1);
	}
	else
	{
		m_parameters = "";
	}
}

