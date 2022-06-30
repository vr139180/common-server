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
};

//xml实现，只支持check检查。不支持submit时对数据的修改。
class ConditionsMetaXml : public IConditionsMeta
{
	friend class IConditionsMeta;
private:
	ConditionsMetaXml();

public:
	virtual bool load_from_xml(tinyxml2::XMLElement* e);

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

private:
	std::string check_fun_;
	std::string submit_fun_;
};

#endif //__CONDITIONSMETA_H__
