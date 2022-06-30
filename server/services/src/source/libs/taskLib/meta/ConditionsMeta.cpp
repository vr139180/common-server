#include <taskLib/meta/ConditionsMeta.h>

//--------------------------------IConditionsMeta------------------------------
IConditionsMeta::IConditionsMeta()
{
}

IConditionsMeta::~IConditionsMeta()
{
}

IConditionsMeta* IConditionsMeta::build_from_xml(tinyxml2::XMLElement* e, std::string impl)
{
	IConditionsMeta* pc = 0;
	if (impl == "lua")
		pc = new ConditionMetaLua();
	else if (impl == "xml")
		pc = new ConditionsMetaXml();

	if (pc == 0)
		return 0;

	std::unique_ptr<IConditionsMeta> ptr(pc);
	if (!pc->load_from_xml(e))
		return 0;

	return ptr.release();
}

bool IConditionsMeta::load_from_xml(tinyxml2::XMLElement* e)
{
	return true;
}

//--------------------------------ConditionMetaLua------------------------------
ConditionMetaLua::ConditionMetaLua() :IConditionsMeta()
{
}

bool ConditionMetaLua::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!IConditionsMeta::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* c = e->FirstChildElement("lua-check");
	if (c != 0)
		check_fun_ = XmlUtil::GetXmlText(c);

	tinyxml2::XMLElement* s = e->FirstChildElement("lua-confirm");
	if (s != 0)
		submit_fun_ = XmlUtil::GetXmlText(s);

	return true;
}

//--------------------------------ConditionsMetaXml------------------------------
ConditionsMetaXml::ConditionsMetaXml() :IConditionsMeta()
{
}

bool ConditionsMetaXml::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!IConditionsMeta::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* c = e->FirstChildElement("xml-check");
	if (c == 0)
		return false;

	TaskXmlCondition* pcond = TaskXmlCondition::build_taskcondition(c, 0);
	if (pcond == 0)
		return false;

	check_cond_.reset(pcond);

	return true;
}
