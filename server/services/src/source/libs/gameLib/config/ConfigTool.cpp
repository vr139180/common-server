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
