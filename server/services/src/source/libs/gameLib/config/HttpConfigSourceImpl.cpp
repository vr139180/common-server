#include "gameLib/config/HttpConfigSourceImpl.h"

#include <cmsLib/Log.h>
#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/serialization/base64/base64.h>

HttpConfigSourceImpl::HttpConfigSourceImpl(void)
{
}

HttpConfigSourceImpl::~HttpConfigSourceImpl(void)
{
}

bool HttpConfigSourceImpl::init_source( std::map< std::string, std::string> prop)
{
	//设置配置文件根目录
	url_ =prop["url"];
	if( url_.size() == 0)
		return false;

	return true;
}

void HttpConfigSourceImpl::uninit_source()
{

}

std::string HttpConfigSourceImpl::get_fromhttp( const char* fun, const char* path, const char* ext)
{
	std::string ret ="";

	std::string query ="";
	query += "fun=";
	query += fun;
	query += "&path=";
	query += path;
	query += "&fext=";
	query += ext;

	HttpUrl url( url_ + "/svrconfig.php");
	url.SetQuery( query);
	HttpResponse response;
	HttpStringWrite writer;
	url.SetScheme( "http");

	HttpHeaders hd;
	hd.m_headers.push_back( "Content-type:text/plain; charset=utf-8");

	if( http_client.HttpGET( url, response, writer, hd))
	{
		ret =writer.GetBody();
	}

	return ret;
}

std::string HttpConfigSourceImpl::get_txtfilecontent( const char* filename, bool deployfile)
{
	std::string p ="";

	if( deployfile)
	{
		std::string query ="";
		query += "fun=filecontent";
		query += "&path=";
		query += filename;

		HttpUrl url( url_ + "/svrconfig.php");
		url.SetQuery( query);

		HttpResponse response;
		HttpStringWrite writer;
		url.SetScheme( "http");

		HttpHeaders hd;
		hd.m_headers.push_back( "Content-type:text/plain; charset=utf-8");

		if( http_client.HttpGET( url, response, writer, hd))
		{
			p =writer.GetBody();
		}
	}
	else
	{
		std::string txturl =url_;
		txturl +="/datas";
		txturl += filename;
		HttpUrl url( txturl);

		HttpResponse response;
		HttpStringWrite writer;
		url.SetScheme( "http");

		HttpHeaders hd;
		hd.m_headers.push_back( "Content-type:text/plain; charset=utf-8");

		if( http_client.HttpGET( url, response, writer, hd))
		{
			p =writer.GetBody();
		}
	}

	return p;
}

void HttpConfigSourceImpl::get_filelist( const char* path, const char* ext, std::list< std::string>& ret)
{
	std::string p =get_fromhttp( "filelist", path, ext);

	size_t pos =p.find( "<c>");
	size_t epos =p.rfind( "</c>");
	if( pos == std::string::npos || epos == std::string::npos)
		return;

	pos += 3;
	p =p.substr( pos, epos - pos);

	ShareUtil::splitstr2str( p.c_str(), ";", ret);
}

void HttpConfigSourceImpl::get_subdirlist( const char* path, std::list< std::string>& ret)
{

}
