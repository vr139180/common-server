#ifndef __HTTP_READER_H__
#define __HTTP_READER_H__

#include <string>
#include <vector>

class HttpReader
{
public:
	HttpReader(){}
	virtual ~HttpReader(){}

	virtual unsigned int Read( void* pBuffer, unsigned int bufferSize) = 0;

	virtual unsigned int GetSize() = 0;
};

#endif //__HTTP_READER_H__