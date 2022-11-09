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

#ifndef __CONFIGTOOL_H__
#define __CONFIGTOOL_H__

#include <gameLib/config/IConfigContentSource.h>

class ConfigTool
{
private:
	ConfigTool();

public:
	static ConfigTool& get_instance();

	~ConfigTool();

	bool init_configtool( std::string type, std::map< std::string, std::string> prop);

	std::string get_txtfilecontent( const char* filename, bool deployfile =false){
		if( conf_src_)
			return conf_src_->get_txtfilecontent( filename, deployfile);
		else
			return "";
	}

	void get_filelist( const char* path, const char* ext, std::list< std::string>& ret){
		if( conf_src_)
			conf_src_->get_filelist( path, ext, ret);
	}

	void get_subdirlist( const char* path, std::list< std::string>& ret){
		if( conf_src_)
			conf_src_->get_subdirlist( path, ret);
	}

private:
	IConfigContentSource*	conf_src_;
};

#endif	//__CONFIGTOOL_H__
