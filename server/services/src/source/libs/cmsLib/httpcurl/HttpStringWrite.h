#ifndef __HTTP_STRING_WRITE_H__
#define __HTTP_STRING_WRITE_H__

#include <string>
#include <cmsLib/httpcurl/HttpWrite.h>

class HttpStringWrite : public HTTPWriter
{
public:
	HttpStringWrite()
	{
		m_strwrite.clear();
	}

	virtual ~HttpStringWrite()
	{
		m_strwrite.clear();
	}

	const char* GetBody() const
	{
		return m_strwrite.c_str();
	}

	virtual bool Write(const void* pBuffer, unsigned int bufferSize)
	{
		m_strwrite += (char*)(pBuffer);

		return true;
	}

private:
	std::string m_strwrite;
};

#endif //__HTTP_STRING_WRITE_H__