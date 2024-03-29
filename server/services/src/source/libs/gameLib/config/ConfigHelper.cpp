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

#include "gameLib/config/ConfigHelper.h"

#include <gameLib/config/IConfigContentSource.h>

#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/util/FileUtil.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/config/ConfigTool.h>

ConfigHelper& ConfigHelper::instance() {
	static ConfigHelper s_instance;
	return s_instance;
}

ConfigHelper::ConfigHelper():
	type_(NETSERVICE_TYPE::ERK_SERVICE_NONE)
{
}

ConfigHelper::~ConfigHelper(void)
{
	release();
}

void ConfigHelper::release()
{
}

bool ConfigHelper::init_config(NETSERVICE_TYPE mytype)
{
	this->type_ = mytype;

	if( !load_svrno())
		return false;

	if( !ConfigTool::get_instance().init_configtool( cnfprop_["cfgtype"], cnfprop_))
		return false;

	if (!load_globaloption())
		return false;

	return true;
}

bool ConfigHelper::load_svrno()
{
    std::string cnt =FileUtil::get_filecontent( OSSystem::work_path2_.c_str(), "svrno");
	if( cnt.size() == 0)
		return false;

	tinyxml2::XMLDocument doc;
	if( !XmlUtil::loadfromstring( cnt.c_str(), doc))
		return false;

	tinyxml2::XMLElement* root =doc.RootElement();

	nets_ =XmlUtil::GetXmlAttrInt( root, "neths", 2);
	this->ip_ = XmlUtil::GetXmlAttrStr(root, "ip", "127.0.0.1");
	port_ = XmlUtil::GetXmlAttrInt(root, "port", 8888);

	std::string srct =XmlUtil::GetXmlAttrStr( root, "confsrc", "filesrc");
	cnfprop_["cfgtype"] =srct;

	if( srct == "filesrc")
	{
		tinyxml2::XMLElement* xcnf =root->FirstChildElement( "filesrc");
		if( xcnf == 0)
			return false;

		std::string v =XmlUtil::GetXmlAttrStr( xcnf, "datadir", "./datas");
		cnfprop_["dirbase"] = v;
	}
	else if( srct == "httpsrc")
	{
		tinyxml2::XMLElement* xcnf =root->FirstChildElement( "httpsrc");
		if( xcnf == 0)
			return false;

		std::string v =XmlUtil::GetXmlAttrStr( xcnf, "url", "");
		if( v == "")
			return false;

		cnfprop_["url"] = v;
	}
	else
		return false;

	tinyxml2::XMLElement* pexts = root->FirstChildElement("exts");
	if (pexts)
	{
		for (tinyxml2::XMLElement* pe = pexts->FirstChildElement("ext"); pe != 0; pe = pe->NextSiblingElement("ext"))
		{
			std::string k = XmlUtil::GetXmlAttrStr(pe, "k", "");
			std::string v = XmlUtil::GetXmlAttrStr(pe, "v", "");
			if (k == "" || v == "")
				continue;
			ShareUtil::to_lower(k);
			ShareUtil::to_lower(v);
			extprop_[k] = v;
		}
	}

	return true;
}

std::string ConfigHelper::find_ext(const char* name)
{
	boost::unordered_map<std::string, std::string>::iterator fiter = extprop_.find(name);
	if (fiter == extprop_.end())
		return "";
	return fiter->second;
}

bool ConfigHelper::load_globaloption()
{
	std::string fstr = "/system/global.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return false;

	tinyxml2::XMLElement* root = doc.RootElement();
	global_.svrnum_min = XmlUtil::GetXmlAttrInt(root, "svrnum_min", 1000);

	global_.eureka_ = XmlUtil::GetXmlAttrStr(root, "eureka", "http://127.0.0.1:8080/gate/eureka");

	return true;
}

EurekaConfig* ConfigHelper::load_eureka_config()
{
	EurekaConfig* config = new EurekaConfig();
	std::unique_ptr<EurekaConfig> xptr(config);

	std::string fstr = "/system/eureka_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt( root, "loopnum", 100);

	tinyxml2::XMLElement* rds = root->FirstChildElement("redis");
	if (rds == 0)
		return 0;

	config->redis_.load_from_xml(rds);

	xptr.release();
	return config;
}

