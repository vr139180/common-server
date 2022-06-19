#ifndef __CONDITIONELEMENT__H__
#define __CONDITIONELEMENT__H__

#include <assert.h>

class ConditionContext;
namespace tinyxml2 {
	class XMLElement;
}

//��������������Ϸ�ʽ
enum CONDITION_CONNECTOR
{
	CONDITION_CONNECTOR_AND =0,
	CONDITION_CONNECTOR_OR,
	CONDITION_CONNECTOR_NOT,
};

//������֧�ֵ������
enum CONDITION_OPERATOR
{
	// >
	CONDITION_OPERATOR_GREAT =0,
	// >=
	CONDITION_OPERATOR_GREATEQUAL,
	// ==
	CONDITION_OPERATOR_EQUAL,
	// <
	CONDITION_OPERATOR_LESS,
	// <=
	CONDITION_OPERATOR_LESSEQUAL,
	// !=
	CONDITION_OPERATOR_NOTEQUAL,
	// > x <
	CONDITION_OPERATOR_BETWEEN,
};

/**
* @class ConditionElement
* 
* @brief �����ж��ڵ����
**/
class ConditionElement
{
public:
	/**
	* ��xml�ڵ��й���condition
	* @param e
	* @return
	**/
	static ConditionElement* construct_condition( tinyxml2::XMLElement* e);

public:
	/**
	* �Ƿ���Ҷ�ӽڵ㣬ֻ��Ҷ�ӽڵ�����߼�����
	* @return
	**/
	virtual bool	is_leaf() =0;

	/**
	* �������������趨�������߼�
	* @param context
	* @return
	**/
	virtual bool	condition_calcuate( ConditionContext& context) =0;
};

#endif	//__CONDITIONELEMENT__H__
