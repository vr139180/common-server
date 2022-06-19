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
* @brief ���������ж����ֵ
**/
class ConditionValue
{
public:
	/**
	* �������õ�ֵ
	**/
	virtual PropertyOperatorValue get_conditionvalue( ConditionContext& context) =0;
};

/**
* @class ConditionValueConst
* 
* @brief ��װ�˳���ֵ
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
* @brief ��װ�˶�������
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
