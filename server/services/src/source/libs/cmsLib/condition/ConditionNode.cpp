#include "cmsLib/condition/ConditionNode.h"

#include "cmsLib/util/XmlUtil.h"

#include "cmsLib/condition/ConditionLeaf.h"

ConditionNode::ConditionNode( ConditionElement* l, ConditionElement* r, CONDITION_CONNECTOR connector):
left_( l),
right_( r),
connector_( connector)
{
	assert( left_ != 0);

	if( connector_ != CONDITION_CONNECTOR_NOT)
		assert( right_ != 0);
}

ConditionNode::ConditionNode( tinyxml2::XMLElement* e)
{
	std::string conn =XmlUtil::GetXmlAttrStr( e, "connector", "and");
	CONDITION_CONNECTOR ctype =CONDITION_CONNECTOR_AND;
	if( conn == "and")
		ctype =CONDITION_CONNECTOR_AND;
	else if( conn == "or")
		ctype =CONDITION_CONNECTOR_OR;
	else
		ctype =CONDITION_CONNECTOR_NOT;

	ConditionElement *lnode =0, *rnode =0;

	tinyxml2::XMLElement* l1 =e->FirstChildElement( "condition");
	std::string tye =XmlUtil::GetXmlAttrStr( l1, "type", "node");
	if( tye == "node")
		lnode =new ConditionNode( l1);
	else if( tye == "leaf")
		lnode =new ConditionLeaf( l1);

	tinyxml2::XMLElement* r1 =0;
	if( l1)
	{
		r1 =l1->NextSiblingElement( "condition");
		tye =XmlUtil::GetXmlAttrStr( r1, "type", "node");
		if( tye == "node")
			rnode =new ConditionNode( r1);
		else if( tye == "leaf")
			rnode =new ConditionLeaf( r1);
	}

	left_ =lnode;
	right_ =rnode;
	connector_ =ctype;

	assert( left_ != 0);
	if( connector_ != CONDITION_CONNECTOR_NOT)
		assert( right_ != 0);
}

ConditionNode::~ConditionNode()
{
	if( left_)
		delete left_;
	left_ =0;

	if( right_)
		delete right_;
	right_ =0;
}

bool ConditionNode::condition_calcuate( ConditionContext& context)
{
	bool lret =true, rret =true;
	if( left_)
		lret =left_->condition_calcuate( context);
	if( right_)
		rret =right_->condition_calcuate( context);

	if( connector_ == CONDITION_CONNECTOR_NOT)
		return !lret;
	else if( connector_ == CONDITION_CONNECTOR_AND)
		return lret & rret;
	else if( connector_ == CONDITION_CONNECTOR_OR)
		return lret || rret;
	else
	{
		assert( false && "ConditionNode::condition_calcuate Not support condition connector type");
		return false;
	}
}
