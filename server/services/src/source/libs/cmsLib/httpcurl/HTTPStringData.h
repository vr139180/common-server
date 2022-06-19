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