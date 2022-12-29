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
#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <curl/curl.h>
#include <string>

#include <cmsLib/httpcurl/HttpUtilities.h>
#include <cmsLib/httpcurl/HttpWrite.h>
#include <cmsLib/httpcurl/HttpResponse.h>
#include <cmsLib/httpcurl/HTTPData.h>
#include <cmsLib/httpcurl/HttpUrl.h>
#include <cmsLib/httpcurl/HttpReader.h>
#include <cmsLib/httpcurl/HTTPCurlRequest.h>
#include <cmsLib/httpcurl/HTTPProxy.h>
#include <cmsLib/httpcurl/HttpStringWrite.h>
#include <cmsLib/httpcurl/HTTPStringData.h>

class HttpClient
{
public:
	static void curl_init();
	static void curl_uninit();

public:
	HttpClient();
	~HttpClient();

	void ClearSetOption();

	bool HttpGET(const HttpUrl& url, HttpResponse& response, HTTPWriter& writer, const HttpHeaders& httpHeaders = HttpHeaders());

	bool HttpPOST(const HttpUrl& url, HTTPData& body, HttpResponse& response, HTTPWriter& writer, const HttpHeaders& httpHeaders = HttpHeaders());

	bool HttpPUT(const HttpUrl& url, HttpResponse& response, HttpReader& reader, const HttpHeaders& httpHeaders = HttpHeaders());

	enum HTTPTransferState
	{
		NotStarted,
		Pending,
		Completed
	};

	HTTPProxy* GetHttpProxy() { return &m_snsProxy; };

	void SetHttpProxy( HTTPProxy proxy) { m_snsProxy = proxy; };

private:
	HTTPCurlRequest*	m_pCurlRequest;
	HTTPProxy			m_snsProxy;
	
	CURLcode		curl_lasterror;
};

#endif //__HTTP_CLIENT_H__