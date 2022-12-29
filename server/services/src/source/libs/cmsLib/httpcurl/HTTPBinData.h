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
#ifndef __HTTP_BIN_DATA_H__
#define __HTTP_BIN_DATA_H__

#include <cmsLib/httpcurl/HTTPData.h>

class HttpBinData : public HTTPData
{
public:
	explicit HttpBinData(const char* data, size_t size)
		: m_data( 0), m_size(size), m_pos(0)
	{
		if (size > 0)
		{
			m_data = (char*)malloc(size);
			memcpy( m_data,data,size);
		}
	}

	virtual ~HttpBinData() {
		if (m_data != NULL)
		{
			free(m_data);
			m_data = NULL;
			m_size = 0;
			m_pos = 0;
		}
	}

    unsigned int GetContentLength() const
	{
		return m_size;
	}

	void Write(const char* pBuffer, unsigned int bufferSize)
	{
		if( m_data != 0)
		{	
			m_data = (char*)realloc(m_data, m_size + bufferSize);
			memcpy(m_data + m_size,pBuffer,bufferSize);
			m_size+=bufferSize;
		}
		else
		{
			m_size+=bufferSize;
			m_data = (char*)malloc(m_size);
			memcpy(m_data,pBuffer,m_size);
		}
	}
	
	unsigned int Read(void* pBuffer, unsigned int bufferSize)
	{
		if (m_pos >= m_size)
			return 0;

		unsigned int toRead = min( bufferSize, m_size - m_pos);
		std::memcpy(pBuffer, m_data, toRead);
		m_pos += toRead;
		return toRead;
	}

	bool IsFinish() const
	{
		return m_pos == m_size;
	}

private:
	char *			m_data;
	unsigned int	m_size;
	unsigned int	m_pos;
};

#endif // __HTTP_BIN_DATA_H__
