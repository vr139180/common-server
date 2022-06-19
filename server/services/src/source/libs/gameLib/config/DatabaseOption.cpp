#include <gameLib/config/DatabaseOption.h>

#include <cmsLib/util/XmlUtil.h>

using namespace config;

DatabaseOption::DatabaseOption()
{
}

void DatabaseOption::load_from_xml(tinyxml2::XMLElement* el)
{
	dbname_ = XmlUtil::GetXmlAttrStr(el, "db", "cms");
	dbip_ = XmlUtil::GetXmlAttrStr(el, "ip", "tcp://127.0.0.1:3306");
	dbuser_ = XmlUtil::GetXmlAttrStr(el, "user", "root");
	dbpwd_ = XmlUtil::GetXmlAttrStr(el, "pwd", "123456");
}
