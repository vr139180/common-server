#ifndef __CONDITIONLEAF__H__
#define __CONDITIONLEAF__H__

#include "cmsLib/condition/ConditionElement.h"
#include "cmsLib/condition/ConditionValue.h"

/**
* @class ConditionLeaf
* 
* @brief 条件判定叶子节点
**/
class ConditionLeaf : public ConditionElement
{
public:
	ConditionLeaf();
	ConditionLeaf( tinyxml2::XMLElement* e);
	virtual ~ConditionLeaf();

	virtual bool is_leaf(){ return true;}
	virtual bool condition_calcuate( ConditionContext& context);

protected:
	//运算符
	CONDITION_OPERATOR	operator_;
	//运算符左右值 x_只在between中使用
	ConditionValue		*left_, *right_, *x_;
};

#endif	//__CONDITIONLEAF__H__
