#ifndef __HTTP_DATA_H__
#define __HTTP_DATA_H__
	
class HTTPData
{
public:
    virtual ~HTTPData(){}
	
    virtual unsigned int GetContentLength() const = 0;

	virtual unsigned int Read( void* pBuffer, unsigned int bufferSize) = 0;
};

#endif // __HTTP_DATA_H__
