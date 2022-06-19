#ifndef __HTTP_HEADERS_H__
#define __HTTP_HEADERS_H__

#include <string>
#include <vector>

class HttpHeaders
{
public:
	HttpHeaders(){}
	~HttpHeaders(){}

	void Clear()
	{
		m_headers.clear();
	}

	std::vector<std::string>	m_headers;
};

#endif //__HTTP_HEADERS_H__
