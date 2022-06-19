#include <gameLib/config/ConfigTool.h>

#include "gameLib/config/FileConfigSourceImpl.h"
#include "gameLib/config/HttpConfigSourceImpl.h"
#include <cmsLib/httpcurl/HttpClient.h>

ConfigTool& ConfigTool::get_instance()
{
	static ConfigTool s_instance_;
	return s_instance_;
}

ConfigTool::ConfigTool():
	conf_src_( 0)
{
	HttpClient::curl_init();
}

ConfigTool::~ConfigTool()
{
	if( conf_src_)
	{
		conf_src_->uninit_source();
		delete conf_src_;

		conf_src_ =0;
	}
}

bool ConfigTool::init_configtool( std::string type, std::map< std::string, std::string> prop)
{
	if( type == "filesrc")
		conf_src_ =new FileConfigSourceImpl();
	else if( type == "httpsrc")
		conf_src_ =new HttpConfigSourceImpl();

	if( conf_src_ == 0)
		return false;
	else if( !conf_src_->init_source( prop))
		return false;

	return true;
}
