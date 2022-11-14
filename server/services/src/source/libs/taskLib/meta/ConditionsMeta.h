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

#ifndef __CONDITIONSMETA_H__
#define __CONDITIONSMETA_H__

#include <cmsLib/util/XmlUtil.h>
#include <taskLib/meta/TaskXmlCondition.h>

class IConditionsMeta
{
protected:
	IConditionsMeta();

public:
	static IConditionsMeta* build_from_xml(tinyxml2::XMLElement* e, std::string impl);
	virtual ~IConditionsMeta();

	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	virtual bool is_xmlcondition() { return false; }
	virtual bool is_luacondition() { return false; }
};

//xml实现，只支持check检查。不支持submit时对数据的修改。
class ConditionsMetaXml : public IConditionsMeta
{
	friend class IConditionsMeta;
private:
	ConditionsMetaXml();

public:
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	virtual bool is_xmlcondition() { return true; }

	TaskXmlCondition* get_condition() { return check_cond_.get(); }

private:
	std::shared_ptr<TaskXmlCondition>	check_cond_;
};

class ConditionMetaLua : public IConditionsMeta
{
	friend class IConditionsMeta;
private:
	ConditionMetaLua();

public:
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

	virtual bool is_luacondition() { return true; }

	const char* get_checkfun() { return check_fun_.c_str(); }
	bool need_check() { return check_fun_.size() > 0; }
	const char* get_confirmfun() { return confirm_fun_.c_str(); }
	bool need_confirm() { return confirm_fun_.size() > 0; }

protected:
	void create_luacall(std::string& funs);

private:
	std::string check_fun_;
	std::string confirm_fun_;
};

#endif //__CONDITIONSMETA_H__
