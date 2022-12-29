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
