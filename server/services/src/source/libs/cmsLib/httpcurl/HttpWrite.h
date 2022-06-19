#ifndef __HTTP_WRITER_H__
#define __HTTP_WRITER_H__

class HTTPWriter
{
public:
	HTTPWriter(){}
	virtual ~HTTPWriter(){}

	virtual bool Write( const void* pBuffer, unsigned int bufferSize) = 0;

};

#endif //__HTTP_WRITER_H__
