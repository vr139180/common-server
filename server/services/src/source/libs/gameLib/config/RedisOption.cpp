#include "gameLib/config/RedisOption.h"

#include <cmsLib/util/XmlUtil.h>

using namespace config;

RedisOption::RedisOption()
{
}

void RedisOption::load_from_xml(tinyxml2::XMLElement* el)
{
	name_ = XmlUtil::GetXmlAttrStr(el, "name");
	ip_ = XmlUtil::GetXmlAttrStr(el, "ip", "127.0.0.1");
	port_ = XmlUtil::GetXmlAttrInt(el, "port", 6379);
	auth_ = XmlUtil::GetXmlAttrStr(el, "auth", "");
	db_ = XmlUtil::GetXmlAttrInt(el, "db", 0);
	socket_timeout_ = XmlUtil::GetXmlAttrInt(el, "socket_timeout", 100);
}
