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

private:
	std::string check_fun_;
	std::string submit_fun_;
};

#endif //__CONDITIONSMETA_H__
