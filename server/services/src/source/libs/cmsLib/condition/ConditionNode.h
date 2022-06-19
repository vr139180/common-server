#ifndef __CONDITIONNODE__H__
#define __CONDITIONNODE__H__

#include "cmsLib/condition/ConditionElement.h"

/**
* @class ConditionNode
* 
* @brief �����ж��ӽڵ�
**/
class ConditionNode : public ConditionElement
{
public:
	ConditionNode( ConditionElement* l, ConditionElement* r =0, CONDITION_CONNECTOR connector =CONDITION_CONNECTOR_AND);
	ConditionNode( tinyxml2::XMLElement* e);

	virtual ~ConditionNode();

	virtual bool is_leaf(){ return false;}

	virtual bool condition_calcuate( ConditionContext& context);

	ConditionElement* get_left(){ return left_;}
	ConditionElement* get_right(){ return right_;}

protected:
	//������Ϸ�ʽ
	CONDITION_CONNECTOR	connector_;
	//����
	ConditionElement	*left_, *right_;
};

#endif	//__CONDITIONNODE__H__
