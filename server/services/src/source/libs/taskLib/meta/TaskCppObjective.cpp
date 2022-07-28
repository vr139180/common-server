#include <taskLib/meta/TaskCppObjective.h>


#include <cmsLib/util/ShareUtil.h>
#include <gameLib/LogExt.h>

CPPObjectiveParams::CPPParamValue::CPPParamValue()
{
}

CPPObjectiveParams::CPPParamValue::CPPParamValue(const char* v) :val(v)
{
}

void CPPObjectiveParams::CPPParamValue::setval(const char* v)
{
	val = v;
}

int CPPObjectiveParams::CPPParamValue::intval()
{
	return ShareUtil::atoi(val.c_str());
}

S_INT_64 CPPObjectiveParams::CPPParamValue::int64val()
{
	return ShareUtil::atoi64(val.c_str());
}

const char* CPPObjectiveParams::CPPParamValue::strval()
{
	return val.c_str();
}

CPPObjectiveParams::CPPObjectiveParams()
{
}

CPPObjectiveParams* CPPObjectiveParams::build_cppparams(tinyxml2::XMLElement* e)
{
	CPPObjectiveParams* op = new CPPObjectiveParams();
	if (e == 0)
		return op;

	for (tinyxml2::XMLElement* v = e->FirstChildElement("pm"); v != 0; v = v->NextSiblingElement("pm"))
	{
		std::string key = XmlUtil::GetXmlAttrStr(v, "key", "");
		if (key == "")
			continue;
		std::string val = XmlUtil::GetXmlAttrStr(v, "val", "");
		if (val == "")
			continue;

		CPPObjectiveParams::CPPParamValue cv(val.c_str());
		op->params_[key] = cv;
	}
	
	return op;
}

CPPObjectiveParams::CPPParamValue* CPPObjectiveParams::get_param(const char* key)
{
	if (key == 0)
		return 0;

	boost::unordered_map<std::string, CPPParamValue>::iterator fiter = params_.find(key);
	if (fiter == params_.end())
		return 0;

	CPPParamValue& val = fiter->second;
	return &val;
}

bool CPPObjectiveParams::intval_param(const char* key, int& val)
{
	CPPParamValue* pv = get_param( key);
	if (pv == 0)
		return false;

	val = pv->intval();
	return true;
}

bool CPPObjectiveParams::int64val_param(const char* key, S_INT_64& val)
{
	CPPParamValue* pv = get_param(key);
	if (pv == 0)
		return false;

	val = pv->int64val();
	return true;
}

bool CPPObjectiveParams::strval_param(const char* key, std::string& val)
{
	CPPParamValue* pv = get_param(key);
	if (pv == 0)
		return false;

	val = pv->strval();
	return true;
}