#ifndef __CONDITIONVALUE__H__
#define __CONDITIONVALUE__H__

#include "cmsLib/condition/IPropertyOperator.h"

#include <string>

class ConditionContext;
namespace tinyxml2 {
	class XMLElement;
}

/**
* @class ConditionValue
* 
* @brief 条件计算中定义的值
**/
class ConditionValue
{
public:
	/**
	* 返回设置的值
	**/
	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context) =0;
};

/**
* @class ConditionValueConst
* 
* @brief 封装了常量值
**/
class ConditionValueConst : public ConditionValue
{
public:
	ConditionValueConst( int val);
	ConditionValueConst( float val);
	ConditionValueConst( const char* val);

	ConditionValueConst( tinyxml2::XMLElement* e);

	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context);

protected:
	PropertyOperatorValue	const_value_;
};

/**
* @class ConditionValueObject
* 
* @brief 封装了对象属性
**/
class ConditionValueObject : public ConditionValue
{
public:
	ConditionValueObject( const char* objname, const char* propname);

	ConditionValueObject( tinyxml2::XMLElement* e);

	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context);

protected:
	std::string	objname_;
	std::string  propname_;
};

#endif	//__CONDITIONVALUE__H__
