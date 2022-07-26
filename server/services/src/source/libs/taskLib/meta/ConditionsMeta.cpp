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
	check_fun_ = "";
	confirm_fun_ = "";
}

bool ConditionMetaLua::load_from_xml(tinyxml2::XMLElement* e)
{
	if (!IConditionsMeta::load_from_xml(e))
		return false;

	tinyxml2::XMLElement* c = e->FirstChildElement("lua-check");
	if (c != 0)
	{
		check_fun_ = XmlUtil::GetXmlText(c);
		create_luacall(check_fun_);
	}

	tinyxml2::XMLElement* s = e->FirstChildElement("lua-confirm");
	if (s != 0)
	{
		confirm_fun_ = XmlUtil::GetXmlText(s);
		create_luacall(confirm_fun_);
	}

	return true;
}

void ConditionMetaLua::create_luacall(std::string& funs)
{
	std::stringstream ostr;
	ostr << "function _tsfun_()" << "\r\n";
	ostr << funs.c_str() << "\r\n";
	ostr << "end" << "\r\n";
	ostr << "_tsfun_()" << std::endl;

	funs = ostr.str().c_str();
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
