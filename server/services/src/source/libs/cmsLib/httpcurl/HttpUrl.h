#ifndef __HTTP_URL_H__
#define __HTTP_URL_H__

#include <string>

class HttpUrl
{
public:
	/**
		\brief Construct the SnsHttpUrl.
		\param[in] url A well formated URL as a String.
	*/
	explicit HttpUrl( const std::string &url = "http://localhost/");

	/**
		\brief Destructor
	*/
    ~HttpUrl();
	
	/**
		\brief Convert SnsHttpUrl to its normal presentation.
		\returns The SnsHttpUrl as a String in its normal presentation.
	*/
	const std::string ConvertToString() const;
	
	
	/**
		\brief Get the scheme of the URL.
		\returns The scheme of the URL, without the :// at the end.
	*/
	const std::string &GetScheme() const;
	
	/**
		\brief Get the username of the URL
		\returns The username, if exists, URL escaped.  An empty string otherwise.
	*/
	const std::string &GetUsername() const;
	
	/**
		\brief Get the password for this URL
		\returns The password, if valid, URL escaped.  An empty string otherwise.
		\remarks This function always return an empty string if no username are set.
	*/
	const std::string &GetPassword() const;
	
	/**
		\brief Get the host part of this URL.
		\returns The host name; can be empty.
	*/
	const std::string &GetHost() const;
	
	/**
		\brief Get the port of this URL
		\returns The port used by this URL.
		\remarks A port number of zero means to use the default port for the scheme.
	*/
	unsigned int GetPort() const;
	
	/**
		\brief Get the path of the URL
		\returns The path of the URL, without the leading '/'; Can be empty for '/'
	*/
	const std::string &GetPath() const;
	
	/**
		\brief Get the parameters of the path component.
		\returns The parameter at the end of the path component.
		\remarks Only one parameters is support.  The leading ';' is removed.
	*/
	const std::string &GetParameters() const;
	
	/**
		\brief Get the query string of the URL.
		\returns The query string of the URL, URL escaped, without the leading '?'
	*/
	const std::string &GetQuery() const;
	
	/**
		\brief Get the fragment of the URL.
		\returns The fragment of the URL, if present, without the leading '#'
	*/
	const std::string &GetFragment() const;
	
	/**
		\brief Set the scheme. 
		\param[in] scheme the scheme to be set.
		\remarks Will also clear the port if it is set.
	*/
	void SetScheme(const std::string & scheme);

	/**
		\brief Set the host and the port.
		\param[in] host The host name
		\param[in] port The port used 
		\remarks A port value of zero means the use of the default port for the specified scheme.
	*/
	void SetLocation(const std::string & host, unsigned int port = 0);


	/**
		\brief Rerurn a string that strips schema, location, user name and password
		\internal
	*/
	const std::string StripLocation() const;

	/**
		\brief Set username and password.
		\param[in] username The user name
		\param[in] password The password
		\remarks If you pass an empty string as username or password,
			they will be considered as unset.  There is currently no way to
			set the password or the username to an empty string.  Also, the
			RFC doesn't support having a password with the username unset.
		
		\code
			HTTPURL url(_T("http://example.com/");

			url.SetAuthentification(_T("foo"), _T("secret"))
			ASSERT(url.ConvertToString() == _T("http://foo:secret@example.com/"))

			url.SetAuthentification(_T(""), _T(""))
			ASSERT(url.ConvertToString() == _T("http://example.com/"))

			url.SetAuthentification(_T("foo"), _T(""))
			ASSERT(url.ConvertToString() == _T("http://foo@example.com/"))

			url.SetAuthentification(_T(""), _T("secret"))
			ASSERT(url.ConvertToString() == _T("http://example.com/"))
		\endcode
		
		About the diffrence between empty and unset username and
		passwords, see RFC 1738, section 3.1
		http://www.faqs.org/rfcs/rfc1738.html
	*/
	void SetAuthentification(const std::string & username, const std::string & password = "");
	
	/**
		\brief Set the path of the HTTPURL. This must be a valid URL path.
		\param[in] path A valid URL path.
	*/
	void SetPath(const std::string & path);
	
	/**
		\brief Set the parameters string. Don't add a ';' first.
		\param[in] parameters URL parameters
	*/
	void SetParameters(const std::string &parameters);
	
	/**
		\brief Set the query string.
		\param[in] query The query string to be added to the HTTPURL.
		\remarks You must not add a '?' as the first character of the input parameter.
	*/
    void SetQuery(const std::string &query);

	/**
		\brief Set the fragment string.
		\param[in] fragment The fragment to be added to the HTTPURL.
		\remarks You must not add a '#' as the first character of the input parameter.
	*/
    void SetFragment(const std::string &fragment);

	/**
		\brief Escape unsafe characters from its argument so they can be used inside the HTTPURL.
		\param[in] data String to be escaped.
		\returns The string with safe characters.
		\remarks The only safe characters are a-z, A-Z and 0-9.  Other characters are escaped using the %XX patterns, where XX is the 8-bit encoding of the char.
		\sa Unescape
	*/
    static const std::string Escape(const std::string& data);
	/**
		\brief Unescape characters from its argument so they can be used inside the HTTPURL.
		\param[in] escapedData String with only safe characters to unescape.
		\returns String with potentially unsafe characters.
		\remarks This function will replace %XX pattern with the character equivalent to the hexadecimal of the character.
		\sa Escape
	*/
    static const std::string UnEscape(const std::string& escapedData);

	enum SchemeDelimiter
	{
		UriSchemeNone,
		UriSchemeFile,
		UriSchemeFtp,
		UriSchemeGopher,
		UriSchemeHttp,
		UriSchemeHttps,
		UriSchemeMailto,
		UriSchemeNetPipe,
		UriSchemeNetTcp,
		UriSchemeNews,
		UriSchemeNntp,
		UriSchemeMax
	};

protected:
    void ParseURL(const std::string &url);

private:
	std::string		m_scheme;
	SchemeDelimiter	m_sDelimiter;

	std::string		m_username;
	std::string		m_password;
	std::string		m_host;
	unsigned int	m_port;
	std::string		m_path;
	std::string		m_parameters;
	std::string		m_query;
	std::string		m_fragment;

	char*			m_portbuff;
};

#endif //__HTTP_URL_H__