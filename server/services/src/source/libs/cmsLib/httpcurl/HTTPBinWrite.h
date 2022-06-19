#ifndef __HTTP_BINWRITE_H__
#define __HTTP_BINWRITE_H__

#include <stdio.h>
#include <cmsLib/httpcurl/HttpWrite.h>

class HttpBinWrite : public HTTPWriter
{
public:
	HttpBinWrite()
	{
		m_pBinBuff = NULL;
		m_size = 0;
		m_pos = 0;
		m_result = false;
	}

	~HttpBinWrite()
	{
		if( m_pBinBuff != 0)
			free( m_pBinBuff);
		m_size = 0;
		m_pos = 0;
		m_result = false;
	};

	const void* GetBody( unsigned int & binSize ) const
	{
		binSize = m_size;
		return m_pBinBuff;
	};

	virtual bool Write( const void* pBuffer, unsigned int bufferSize)
	{
		if( m_pBinBuff != 0)
		{
			delete m_pBinBuff;
			m_size = 0;
		}
		
		m_pBinBuff = malloc[bufferSize];
		if( m_pBinBuff != 0)
		{
			memcpy(m_pBinBuff,pBuffer,bufferSize);
			return true;
		}

		return false;
	};

private:
	HttpBinWrite( const HttpBinWrite& writer);
	HttpBinWrite& operator = (const HttpBinWrite& writer);

	void*			m_pBinBuff;
	unsigned int	m_size;
	unsigned int	m_pos;
	bool			m_result;
};

#endif //__HTTP_BINWRITE_H__