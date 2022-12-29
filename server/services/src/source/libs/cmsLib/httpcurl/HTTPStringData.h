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
#ifndef __HTTP_STRING_DATA_H__
#define __HTTP_STRING_DATA_H__

#include <cmsLib/httpcurl/HTTPData.h>
#include <string>
#include <string.h>

class HttpStringData : public HTTPData
{
public:
	explicit HttpStringData( const std::string& content)
		: m_content(content), m_readSize(0)
	{
	}

	HttpStringData(const HttpStringData& data)
		: HTTPData(data)
		, m_content(data.m_content)
		, m_readSize(0)
	{
	}

	HttpStringData& operator = (const HttpStringData& data)
	{
		m_content = data.m_content;
		m_readSize = 0;

		return *this;
	}

	virtual ~HttpStringData(){}

	unsigned int GetContentLength() const
	{
		return static_cast<unsigned int>(m_content.length());
	}

	unsigned int Read( void* pBuffer, unsigned int bufferSize)
	{
		unsigned int toRead = static_cast<unsigned int>(m_content.length()) - m_readSize;

		if( toRead > bufferSize)
			toRead = bufferSize;

		memcpy( pBuffer, m_content.c_str() + m_readSize, toRead);

		m_readSize += toRead;

		return toRead;
	}

private:
	std::string		m_content;
	unsigned int	m_readSize;
};

#endif //__HTTP_STRING_DATA_H__