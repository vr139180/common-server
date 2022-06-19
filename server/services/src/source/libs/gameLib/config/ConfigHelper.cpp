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

	cpunum_ =XmlUtil::GetXmlAttrInt( root, "cpu", 1);
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

	return true;
}

bool ConfigHelper::load_globaloption()
{
	std::string fstr = "/system/global.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return false;

	tinyxml2::XMLElement* root = doc.RootElement();
	global_.svrnum_min = XmlUtil::GetXmlAttrInt(root, "svrnum_min", 88);

	global_.eip = XmlUtil::GetXmlAttrStr(root, "eip", "127.0.0.1");
	global_.eport = XmlUtil::GetXmlAttrInt(root, "eport", 8888);

	return true;
}

EurekaConfig* ConfigHelper::load_eureka_config()
{
	EurekaConfig* config = new EurekaConfig();
	std::auto_ptr<EurekaConfig> xptr(config);

	std::string fstr = "/system/eureka_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt( root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	tinyxml2::XMLElement* rds = root->FirstChildElement("redis");
	if (rds == 0)
		return 0;

	config->redis_.load_from_xml(rds);

	xptr.release();
	return config;
}

