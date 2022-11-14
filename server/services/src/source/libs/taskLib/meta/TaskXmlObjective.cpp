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

#include <taskLib/meta/TaskXmlObjective.h>

#include <cmsLib/util/ShareUtil.h>
#include <gameLib/LogExt.h>

XmlObjectiveParams::XmlParamValue::XmlParamValue()
{
}

XmlObjectiveParams::XmlParamValue::XmlParamValue(const char* v):val(v)
{
}

void XmlObjectiveParams::XmlParamValue::setval(const char* v)
{
	val = v;
}

int XmlObjectiveParams::XmlParamValue::intval()
{
	return ShareUtil::atoi(val.c_str());
}

S_INT_64 XmlObjectiveParams::XmlParamValue::int64val()
{
	return ShareUtil::atoi64(val.c_str());
}

const char* XmlObjectiveParams::XmlParamValue::strval()
{
	return val.c_str();
}

XmlObjectiveParams::XmlObjectiveParams()
{
}

XmlObjectiveParams* XmlObjectiveParams::build_xmlparams(const char* str)
{
	std::list<std::string> vs;
	if (!ShareUtil::splitstr2str(str, ",", vs))
		return 0;

	XmlObjectiveParams* op = new XmlObjectiveParams();
	for (std::list<std::string>::iterator iter = vs.begin(); iter != vs.end(); ++iter)
	{
		std::string& xs = (*iter);
		XmlObjectiveParams::XmlParamValue v(xs.c_str());
		op->params_.push_back(v);
	}

	return op;
}

XmlObjectiveParams::XmlParamValue* XmlObjectiveParams::get_param(int index)
{
	if (index < 1 || index > params_.size())
		return 0;

	XmlParamValue& val = params_[index - 1];
	return &val;
}

bool XmlObjectiveParams::intval_param(int index, int& val)
{
	XmlParamValue* pv = get_param(index);
	if (pv == 0)
		return false;

	val = pv->intval();
	return true;
}

bool XmlObjectiveParams::int64val_param(int index, S_INT_64& val)
{
	XmlParamValue* pv = get_param(index);
	if (pv == 0)
		return false;

	val = pv->int64val();
	return true;
}

bool XmlObjectiveParams::strval_param(int index, std::string& val)
{
	XmlParamValue* pv = get_param(index);
	if (pv == 0)
		return false;

	val = pv->strval();
	return true;
}

//---------------------------------------------XmlObjectvie-------------------------------
XmlObjectvie::XmlObjectvie():fun_params_(0)
, oper_params_(0)
{
}

XmlObjectvie::~XmlObjectvie()
{
	release();
}

void XmlObjectvie::release()
{
	if (fun_params_ != 0)
		delete fun_params_;
	fun_params_ = 0;

	if (oper_params_ != 0)
		delete oper_params_;
	oper_params_ = 0;
}

XmlObjectvie* XmlObjectvie::build_objective(tinyxml2::XMLElement* e)
{
	XmlObjectvie* obj = new XmlObjectvie();
	std::unique_ptr<XmlObjectvie> ptr(obj);

	obj->trigger_ = (eTriggerType)XmlUtil::GetXmlAttrInt(e, "trigger");
	obj->fun_name_ = XmlUtil::GetXmlAttrStr(e, "fun");
	std::string funpm = XmlUtil::GetXmlAttrStr(e, "funpm");
	obj->fun_params_ = XmlObjectiveParams::build_xmlparams(funpm.c_str());
	std::string operpm = XmlUtil::GetXmlAttrStr(e, "operpm");
	obj->oper_params_ = XmlObjectiveParams::build_xmlparams(operpm.c_str());

	std::string oper = XmlUtil::GetXmlAttrStr(e, "oper");
	if (oper == "great")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_Great;
	else if (oper == "egreat")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_GreatE;
	else if (oper == "less")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_Less;
	else if (oper == "eless")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_LessE;
	else if (oper == "equal")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_Equal;
	else if (oper == "nequal")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_NotEqual;
	else if (oper == "between")
		obj->oper_ = XmlObjectiveOperType::XmlObjectiveOperType_Between;
	else
	{
		logError(out_runtime, "load xml objective oper type:%s error", oper.c_str());
		return 0;
	}

	if (obj->oper_ == 0)
	{
		logError(out_runtime, "load xml objective , not set operpm");
		return 0;
	}

	if (obj->oper_ == XmlObjectiveOperType::XmlObjectiveOperType_Between)
	{
		if (obj->oper_params_->size() != 2)
		{
			logError(out_runtime, "load xml objective operpm must 2 parameters");
			return 0;
		}
	}
	else
	{
		if (obj->oper_params_->size() != 1)
		{
			logError(out_runtime, "load xml objective operpm must 1 parameters");
			return 0;
		}
	}

	return ptr.release();
}

bool XmlObjectvie::check_result(S_INT_64 retval)
{
	S_INT_64 val1;
	if (!oper_params_->int64val_param(1, val1))
		return false;

	if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_Great)
	{
		return retval > val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_GreatE)
	{
		return retval >= val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_Less)
	{
		return retval < val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_LessE)
	{
		return retval <= val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_Equal)
	{
		return retval == val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_NotEqual)
	{
		return retval != val1;
	}
	else if (oper_ == XmlObjectiveOperType::XmlObjectiveOperType_Between)
	{
		S_INT_64 val2;
		if (!oper_params_->int64val_param(1, val2))
			return false;

		return retval > val1 && retval < val2;
	}

	return false;
}
