#ifndef __CONDITIONLEAF__H__
#define __CONDITIONLEAF__H__

#include "cmsLib/condition/ConditionElement.h"
#include "cmsLib/condition/ConditionValue.h"

/**
* @class ConditionLeaf
* 
* @brief �����ж�Ҷ�ӽڵ�
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
	//�����
	CONDITION_OPERATOR	operator_;
	//���������ֵ x_ֻ��between��ʹ��
	ConditionValue		*left_, *right_, *x_;
};

#endif	//__CONDITIONLEAF__H__
