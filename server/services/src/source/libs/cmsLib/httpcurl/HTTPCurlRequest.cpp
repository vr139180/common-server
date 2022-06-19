#include <cmsLib/httpcurl/HTTPCurlRequest.h>

#include <cmsLib/httpcurl/HttpUtilities.h>
#include <cmsLib/httpcurl/HttpHeaders.h>
#include <cmsLib/httpcurl/HttpWrite.h>
#include <cmsLib/httpcurl/HttpResponse.h>
#include <cmsLib/httpcurl/HTTPData.h>
#include <cmsLib/httpcurl/HttpUrl.h>
#include <cmsLib/httpcurl/HttpReader.h>
#include <cmsLib/httpcurl/HTTPProxy.h>
#include <curl/curl.h>

#include <stdio.h>

#define QVERIFY_CURL(code) QVERIFY( CheckCurlError(code))

HTTPCurlRequest::HTTPCurlRequest()
    : m_pRequest(NULL)
    , m_lastErrorCode(CURLE_OK)
    , m_pHeaders(NULL)
    , m_userData(NULL)
	, m_completed(false)
{
	m_pRequest = curl_easy_init();
}

HTTPCurlRequest::~HTTPCurlRequest()
{
	if( m_pRequest == NULL)
		return;

	Clear();

	curl_easy_cleanup(m_pRequest);
    m_pRequest = NULL;
}

unsigned int HTTPCurlRequest::GetLastError( const char* pErrorDesc) const
{
    if (pErrorDesc != NULL)
    {
        pErrorDesc = curl_easy_strerror(static_cast<CURLcode>(m_lastErrorCode));
    }
    return m_lastErrorCode;
}

void HTTPCurlRequest::ClearError()
{
    m_lastErrorCode = CURLE_OK;
}

bool HTTPCurlRequest::Reset( HTTPMethod method, const HttpUrl& url)
{
	m_completed = false;

    Clear();
    curl_easy_reset(m_pRequest);

    // Disable SSL certificate verification
    SetOption(CURLOPT_SSL_VERIFYPEER, 0);
    // It is OK even if NULL
//    SetOption(CURLOPT_SHARE, pShareHandle);
	SetOption(CURLOPT_NOSIGNAL, 1);
    // Reset the header writer
//    SetHeaderWriter(&CurlRequestHeaderWriter, &m_respHeaders);

    return (SetMethod(method)&&SetUrl(url));
}

void HTTPCurlRequest::Clear()
{
    ClearError();
    SetOption(CURLOPT_HTTPHEADER, NULL);

    if( m_pHeaders != NULL)
    {
        curl_slist_free_all(reinterpret_cast<curl_slist*>(m_pHeaders));
        m_pHeaders = NULL;
    }
}

bool HTTPCurlRequest::SetMethod( HTTPMethod method)
{
    unsigned int opt(0);

    switch(method)
    {
    case POST:
        opt = CURLOPT_POST;
        break;
    case GET:
        opt = CURLOPT_HTTPGET;
        break;

	case PUT:
		opt = CURLOPT_UPLOAD;
		break;
	default:
		return false;
    }
    
    return SetOption(opt,1);
}

bool HTTPCurlRequest::SetUrl(const HttpUrl& url)
{
    return SetOption(CURLOPT_URL,url.ConvertToString());
}

// Description: Setup a writer for the download of data
bool HTTPCurlRequest::SetWriter( IOCallback pFunc, void* stream)
{
    return
        SetOption(CURLOPT_WRITEFUNCTION, pFunc) &&
        SetOption(CURLOPT_WRITEDATA, stream);
}

// Description: Reset Header Writer
bool HTTPCurlRequest::SetHeaderWriter( IOCallback pFunc, void* stream)
{
    return
        SetOption(CURLOPT_HEADERFUNCTION, pFunc) &&
        SetOption(CURLOPT_HEADERDATA, stream);
}

// Description: Set the reader for uploading data.
bool HTTPCurlRequest::SetReader( IOCallback pFunc, void* stream, unsigned __int64 size)
{
    curl_off_t setSize(size);

	return SetOption(CURLOPT_READFUNCTION, pFunc) &&
        SetOption(CURLOPT_READDATA, stream) &&
        SetOption(CURLOPT_POSTFIELDSIZE_LARGE, setSize) &&
		SetOption(CURLOPT_INFILESIZE, setSize);
}

bool HTTPCurlRequest::SetHeaders(const HttpHeaders& headers)
{
	if (m_pHeaders != NULL)
		return false;
    for(size_t i = 0; i<headers.m_headers.size();i++)
    {
        m_pHeaders = curl_slist_append(reinterpret_cast<curl_slist*>(m_pHeaders), headers.m_headers[i].c_str());
    }
    return SetOption(CURLOPT_HTTPHEADER, m_pHeaders);
}

// Description: Allow redirection
bool HTTPCurlRequest::AllowRedirection(bool allow)
{
    const unsigned int setAllow(allow?1:0);

    return SetOption(CURLOPT_FOLLOWLOCATION, &setAllow);
}

// group=Query options
// Description: Return last status code
unsigned int HTTPCurlRequest::GetResponseCode()
{
    unsigned int result;
    GetInfo(CURLINFO_RESPONSE_CODE, result);

    return result;
}

//const SnsHttpHeaders& SnsHTTPCurlRequest::GetHeaders() const
//{
//    return m_respHeaders;
//}

void* HTTPCurlRequest::GetHandle()
{
    return m_pRequest;
}

template <typename ArgType>
bool HTTPCurlRequest::SetOption( unsigned int option, const ArgType& arg)
{
    if(m_lastErrorCode != CURLE_OK)
		return false;

    m_lastErrorCode = curl_easy_setopt(m_pRequest, static_cast<CURLoption>(option), arg);

    return (m_lastErrorCode == CURLSHE_OK);
}

bool HTTPCurlRequest::SetOption( unsigned int option, const std::string& data)
{
    bool result = SetOption<const char*>(option,data.c_str());
    return result;
}

template <typename ArgType>
bool HTTPCurlRequest::GetInfo( unsigned int info, ArgType& arg)
{
    if(m_lastErrorCode != CURLE_OK)
		return false;

    m_lastErrorCode = curl_easy_getinfo(m_pRequest, static_cast<CURLINFO>(info), &arg);

	return (m_lastErrorCode == CURLSHE_OK);
}

bool HTTPCurlRequest::SetProxy(const HTTPProxy& proxy)
{
	std::string proxyString;
	const std::string& host = proxy.GetProxyName();
	const std::string& usename = proxy.GetUsername();
	const std::string& password = proxy.GetPassword();
	char portbuff[50];
	if(!host.empty())
	{
		if(!usename.empty())
		{
			if (!password.empty())
			{
				proxyString = usename;
				proxyString += ":";
				proxyString += password;
				proxyString += "@";
				proxyString += host;
				proxyString += ":";

#if defined(EW_PLATFORM_WINDOWS)
				_snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#elif defined(EW_PLATFORM_LINUX)
				snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#else
#endif
				//_itot_s(proxy.GetProxyPort(),portbuff,50,10);
				proxyString += portbuff;
			}
			else
			{
				proxyString = usename;
				proxyString += "@";
				proxyString += host;
				proxyString += ":";
#if defined(EW_PLATFORM_WINDOWS)
				_snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#elif defined(EW_PLATFORM_LINUX)
				snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#else
#endif				//_itot_s(proxy.GetProxyPort(),portbuff,50,10);
				proxyString += portbuff;
			}
		}
		else
		{
			proxyString += host;
			proxyString += ":";
#if defined(EW_PLATFORM_WINDOWS)
			_snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#elif defined(EW_PLATFORM_LINUX)
			snprintf( portbuff, 50, "%d", proxy.GetProxyPort());

#else
#endif
			//_itot_s(proxy.GetProxyPort(),portbuff,50,10);
			proxyString += portbuff;
		}
	}
	return SetOption(CURLOPT_PROXY, proxyString);
}

bool HTTPCurlRequest::SetTransferBufferSize( unsigned int bufferSize)
{
	return SetOption(CURLOPT_BUFFERSIZE, bufferSize);
}

bool HTTPCurlRequest::SetTimeout(int timeout)
{ 
	if (!SetOption<long>(CURLOPT_TIMEOUT_MS, (long) timeout))
	{
		return false;
	}

	return SetOption<long>(CURLOPT_CONNECTTIMEOUT_MS, (long) timeout);
}

bool HTTPCurlRequest::SetMaxSendSpeed( unsigned __int64 maxSpeed)
{
    curl_off_t curlMaxSpeed(maxSpeed);
    return SetOption(CURLOPT_MAX_SEND_SPEED_LARGE, curlMaxSpeed);
}

bool HTTPCurlRequest::SetMaxRecvSpeed( unsigned __int64 maxSpeed)
{
    curl_off_t curlMaxSpeed(maxSpeed);
    return SetOption(CURLOPT_MAX_RECV_SPEED_LARGE, curlMaxSpeed);
}

CURLcode HTTPCurlRequest::CurlEasyPerform()
{
	return curl_easy_perform(m_pRequest);
}