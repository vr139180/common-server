#ifndef __HTTP_CURL_REQUEST_H__
#define __HTTP_CURL_REQUEST_H__

#include <cmsLib/GlobalSettings.h>
#include <cmsLib/httpcurl/HttpHeaders.h>
#include <curl/curl.h>

class HttpUrl;
class HTTPProxy;

class HTTPCurlRequest
{
public:

    /** \brief Supported HTTP methods. */
    enum HTTPMethod
    {
        GET,
        POST,
		PUT
    };

    // Equivalent to curl_read/write_callback
    typedef size_t(*IOCallback)( char* buffer, size_t size, size_t nmemb, void* stream);

    /** \brief Default constructor. */
    HTTPCurlRequest();

    /** \brief Destructor. */
    virtual ~HTTPCurlRequest();

	unsigned int GetLastError(const char* pErrorDesc = NULL) const;

    /** \brief Clear the error, to access other function. */
    void ClearError();

    /**
        \brief Sets the method and url of the request.
        \param[in] pShareHandle The cURL share handle.
        \param[in] method The HTTP method.
        \param[in] url The URL.
        \returns True if the method and url have been set, false otherwise.
    */
    bool Reset( HTTPMethod method, const HttpUrl& url);

    /** \brief Clears the memory held by the object. */
    void Clear();

    /**
        \brief Sets a writer for the download of data.
        \param[in] pFunc The callback used by cURL to write the received data.
        \param[in] stream A pointer that will be passed to the callback.
        \returns True if the callback and the pointer have been set, false otherwise.
    */
    bool SetWriter(IOCallback pFunc, void* stream);

    /**
        \brief Sets the reader for uploading data.
        \param[in] pFunc The callback used by cURL to read the data to be sent.
        \param[in] stream A pointer that will be passed to the callback.
        \param[in] size The size of he data to be sent in bytes.
        \returns True if the callback, the pointer and the size have been set, false otherwise.
    */
    bool SetReader( IOCallback pFunc, void* stream, unsigned __int64 size);

    /**
        \brief Allows redirection.
        \param[in] allow A boolean that allows (if true) or denies (if false) redirection.
        \returns True if the redirection option was set successfully, false otherwise.
    */
    bool AllowRedirection( bool allow = true);

    /**
        \brief Sets the headers of the request.
        \param[in] headers The HTTP headers.
        \returns True if the headers have been set, false otherwise.
    */
    bool SetHeaders( const HttpHeaders& headers);

    /**
        \brief Sets the HTTP proxy.
        \param[in] proxy The HTTP proxy settings.
        \returns True if the HTTP proxy settings have been set, false otherwise.
    */
    bool SetProxy(const HTTPProxy& proxy);

	/**
        \brief Sets the transfer buffer size.
        \param[in] bufferSize The transfer buffer size, in bytes.
        \returns True if the transfer buffer size has been set, false otherwise.
    */
	bool SetTransferBufferSize( unsigned int bufferSize);

	/**
        \brief Sets the transfer timeout.
        \param[in] timeout The transfer timeout, in milliseconds.
		\returns True if the transfer timeout has been set, false otherwise.
    */
	bool SetTimeout(int timeout);


    /**
        \brief Set the max speed for a send transfer
        \param[in] maxSpeed The maximum transfer speed in bytes/seconds
    */
    bool SetMaxSendSpeed( unsigned __int64 maxSpeed);

    /**
        \brief Set the max speed for a receive transfer
        \param[in] maxSpeed The maximum transfer speed in bytes/seconds
    */
    bool SetMaxRecvSpeed( unsigned __int64 maxSpeed);


    /**
        \brief Gets last response code.
        \returns The last response code.
    */
    unsigned int GetResponseCode();

    /**
        \brief Gets all headers known by this connection.
        \returns The HTTP headers known by this connection.
    */
//    const SnsHttpHeaders& GetHeaders() const;

    void* GetHandle();

	void  SetCompletedStatus(bool status) { m_completed = status;}
	bool GetCompletedStatus() { return m_completed;}

	CURLcode CurlEasyPerform();

	/**
        \brief Sets the Header Writer
        \param[in] pFunc The callback used by cURL to write the received header.
        \param[in] stream A pointer that will be passed to the callback.
        \returns True if the callback and the pointer have been set, false otherwise.
    */
    bool SetHeaderWriter(IOCallback pFunc, void* stream);

protected:

    /**
        \brief Sets the method type (GET or POST).
        \param[in] method The HTTP method.
        \returns True if the method has been set, false otherwise.
    */
    bool SetMethod( HTTPMethod method);

    /**
        \brief Sets the URL to be used.
        \param[in] url The URL.
        \returns True if the URL has been set, false otherwise.
    */
    bool SetUrl(const HttpUrl& url);

    template <typename ArgType>
    bool SetOption( unsigned int option, const ArgType& arg);
    // Specialization overloading
	bool SetOption( unsigned int option, const std::string& data);
    template <typename ArgType>
    bool GetInfo( unsigned int info, ArgType& arg);

private:
    void* 			m_pRequest;
	unsigned int	m_lastErrorCode;
    void*			m_pHeaders;
    void*			m_userData;
//    SnsHttpHeaders*	m_respHeaders;
	bool			m_completed;
};

#endif // __HTTP_CURL_REQUEST_H__
