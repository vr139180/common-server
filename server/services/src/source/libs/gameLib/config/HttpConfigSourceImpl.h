#ifndef __HTTPCONFIGSOURCEIMPL__H__
#define __HTTPCONFIGSOURCEIMPL__H__

#include <gameLib/config/IConfigContentSource.h>

#include <cmsLib/httpcurl/HttpClient.h>

/**
* @class HttpConfigSourceImpl
* 
* @brief 配置模块数据源本地文件实现
**/
class HttpConfigSourceImpl : public IConfigContentSource
{
public:
	HttpConfigSourceImpl();
	~HttpConfigSourceImpl();

public:
	//-----------------------implement IConfigContentSource------------------------
	/**
	* @see void IConfigContentSource::init_source( std::map< std::string, std::string> prop)
	* FileConfigSourceImpl支持的属性
	* dirbase:	配置文件根目录
	**/
	virtual bool init_source( std::map< std::string, std::string> prop);

	/**
	* @see void IConfigContentSource::uninit_source()
	**/
	virtual void uninit_source();

	/**
	* @see NS_STL::string IConfigContentSource::get_txtfilecontent( const char* path, const char* filename)
	**/
	virtual std::string get_txtfilecontent( const char* filename, bool deployfile =false);

	/**
	* @see void IConfigContentSource::get_filelist( const char* path, const char* ext, std::list< std::string>& ret)
	**/
	virtual void get_filelist( const char* path, const char* ext, std::list< std::string>& ret);

	/**
	* @see void IConfigContentSource::get_subdirlist( const char* path, std::list< std::string>& ret)
	**/
	virtual void get_subdirlist( const char* path, std::list< std::string>& ret);

protected:
	std::string get_fromhttp( const char* fun, const char* path, const char* ext);

private:
	//配置信息根目录
	std::string url_;

	HttpClient http_client;
};

#endif	//__HTTPCONFIGSOURCEIMPL__H__
