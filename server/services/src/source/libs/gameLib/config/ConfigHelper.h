﻿// Copyright 2021 common-server Authors
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

#ifndef __CONFIGHELPER_H__
#define __CONFIGHELPER_H__

#include <map>
#include <list>
#include <string>
#include <vector>

#include <cmsLib/core_type.h>
#include "gameLib/config/GlobalOption.h"
#include "gameLib/config/EurekaConfig.h"
#include <gameLib/service_type.h>

namespace tinyxml2 {
	class XMLElement;
}

class ConfigHelper
{
private:
	ConfigHelper();

public:
	static ConfigHelper& instance();

	~ConfigHelper();

	bool init_config(NETSERVICE_TYPE mytype);

	NETSERVICE_TYPE get_type() { return type_; }

	int get_netthreads(){ return nets_;}
	std::string get_ip() { return ip_; }
	int get_port() { return port_; }

	const boost::unordered_map<std::string, std::string>& get_exts() { return extprop_; }
	std::string find_ext(const char* name);

	config::GlobalOption& get_globaloption() { return global_; }

public:
	//加载eureka配置项
	EurekaConfig* load_eureka_config();
 
protected:
	bool load_svrno();
	bool load_globaloption();

	void release();

private:
	NETSERVICE_TYPE	type_;

	//网络线程数
	int nets_;
	std::string ip_;
	int port_;

	boost::unordered_map<std::string, std::string> extprop_;

	std::map< std::string, std::string> cnfprop_;
	//全局配置
	config::GlobalOption global_;
};

#endif	//__CONFIGHELPER_H__
