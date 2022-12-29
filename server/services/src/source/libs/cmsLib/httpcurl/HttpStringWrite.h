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
		if (bufferSize <= 0)
			return true;

		std::string xstr((char*)pBuffer, bufferSize);
		m_strwrite += xstr;

		return true;
	}

private:
	std::string m_strwrite;
};

#endif //__HTTP_STRING_WRITE_H__