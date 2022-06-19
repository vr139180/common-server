#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <cmsLib/httpcurl/HttpHeaders.h>

class HttpResponse
{
public:
	HttpResponse():m_statusCode(0),m_headers(){}

	~HttpResponse(){ }

	void Clear()
	{
		m_statusCode = 0;
		m_headers.clear();
	}

	unsigned int GetStatusCode() const
	{
		return m_statusCode;
	}

	unsigned int				m_statusCode;
	std::vector<std::string>	m_headers;
};

#endif //__HTTP_RESPONSE_H__
