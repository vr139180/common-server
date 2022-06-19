#include <cmsLib/httpcurl/HttpClient.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

void HttpClient::curl_init()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void HttpClient::curl_uninit()
{
	curl_global_cleanup();
}

HttpClient::HttpClient(void)
{
	m_pCurlRequest = new HTTPCurlRequest();
}

HttpClient::~HttpClient(void)
{
	if( m_pCurlRequest != 0)
		delete m_pCurlRequest;	
}

void HttpClient::ClearSetOption()
{
	if( m_pCurlRequest != NULL)
	{
		m_pCurlRequest->Clear();
	}
}

size_t HTTPClientWriterCallback(char* buffer, size_t size, size_t nmemb, void* stream)
{
	size_t result = 0;
	HTTPWriter* writer = reinterpret_cast<HTTPWriter*>(stream);
	if(writer == NULL)
		return result;

	if (writer->Write(buffer, static_cast<unsigned int>(size*nmemb)))
	{
		result = nmemb;
	}

	return result;
}

size_t HTTPClientHeaderWriter(char* pBuffer, size_t size, size_t nmemb, void* stream)
{
	HttpHeaders* pHeaders = reinterpret_cast<HttpHeaders*>(stream);
	if (pHeaders == NULL)
		return 0;

	unsigned int len = static_cast<unsigned int>(size*nmemb);
	if (len > 2)
	{
		//std::string header = pBuffer;
		//      SnsUInt32 slen = len-2; // Last characters is \r\n
		//      char* strHeader = new char(slen+1);
		//      std::memcpy(strHeader, pBuffer, slen);
		//      strHeader[slen] = '\0';
		//std::string header = strHeader;
		//delete[] strHeader;
		pHeaders->m_headers.push_back(pBuffer);
	}

	return len;
}

size_t HTTPClientPostReaderCallback(char* buffer, size_t size, size_t nmemb, void* stream)
{
	HTTPData* postreader = reinterpret_cast<HTTPData*>(stream);
	if( postreader == NULL)
		return 0;
	// Note: this libcurl version always call read with size = 1
	// We ensure this is true since it can create false positive roundup.
	if(size != 1)
		return 0;

	return postreader->Read(buffer, static_cast<unsigned int>(nmemb));
}

size_t HTTPClientPutReaderCallback(char* buffer, size_t size, size_t nmemb, void* stream)
{
	HttpReader* putreader = reinterpret_cast<HttpReader*>(stream);
	if (putreader == NULL)
		return 0;
	// Note: this libcurl version always call read with size = 1
	// We ensure this is true since it can create false positive roundup.
	if(size != 1)
		return 0;

	return putreader->Read(buffer, static_cast<unsigned int>(nmemb));
}

const unsigned int	DEFAULT_TRANSFER_BUFFER_SIZE = 16*1024;
const int			DEFAULT_TIMEOUT = 60*1000;

bool HttpClient::HttpGET(const HttpUrl& url, HttpResponse& response, HTTPWriter& writer, const HttpHeaders& httpHeaders )
{
	m_pCurlRequest->Reset( HTTPCurlRequest::GET, url);
	m_pCurlRequest->AllowRedirection();
	m_pCurlRequest->SetWriter(HTTPClientWriterCallback,&writer);
	m_pCurlRequest->SetHeaderWriter(&HTTPClientHeaderWriter, &(response.m_headers));
	m_pCurlRequest->SetHeaders(httpHeaders);

	m_pCurlRequest->SetTransferBufferSize(DEFAULT_TRANSFER_BUFFER_SIZE);
	m_pCurlRequest->SetMaxSendSpeed(0);
	m_pCurlRequest->SetMaxRecvSpeed(0);

	m_pCurlRequest->SetTimeout(DEFAULT_TIMEOUT);

	if (!m_snsProxy.GetProxyName().empty())
	{
		m_pCurlRequest->SetProxy(m_snsProxy);
	}

	response.m_statusCode = m_pCurlRequest->CurlEasyPerform();
	return ( response.m_statusCode == CURLE_OK);
}

bool HttpClient::HttpPOST(const HttpUrl& url, HTTPData& body, HttpResponse& response, HTTPWriter& writer, const HttpHeaders& httpHeaders)
{
	m_pCurlRequest->Reset( HTTPCurlRequest::POST, url);

	m_pCurlRequest->SetReader(HTTPClientPostReaderCallback, &body, body.GetContentLength());
	m_pCurlRequest->SetWriter(HTTPClientWriterCallback, &writer);
	m_pCurlRequest->SetHeaderWriter(&HTTPClientHeaderWriter, &(response.m_headers));
	m_pCurlRequest->SetHeaders(httpHeaders);
	m_pCurlRequest->SetTransferBufferSize(DEFAULT_TRANSFER_BUFFER_SIZE);
	m_pCurlRequest->SetMaxSendSpeed(0);
	m_pCurlRequest->SetMaxRecvSpeed(0);

	m_pCurlRequest->SetTimeout(DEFAULT_TIMEOUT);

	if (!m_snsProxy.GetProxyName().empty())
	{
		m_pCurlRequest->SetProxy(m_snsProxy);
	}
	response.m_statusCode = m_pCurlRequest->CurlEasyPerform();
	return ( response.m_statusCode == CURLE_OK);
}

bool HttpClient::HttpPUT(const HttpUrl& url, HttpResponse& response, HttpReader& reader, const HttpHeaders& httpHeaders)
{
	m_pCurlRequest->Reset( HTTPCurlRequest::PUT, url);

	m_pCurlRequest->SetReader(HTTPClientPutReaderCallback, &reader, reader.GetSize());

	m_pCurlRequest->SetHeaders(httpHeaders);

	return true;
}
