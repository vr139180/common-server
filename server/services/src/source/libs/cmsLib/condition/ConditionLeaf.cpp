#include "cmsLib/condition/ConditionLeaf.h"

#include "cmsLib/util/XmlUtil.h"
#include <string>

ConditionLeaf::ConditionLeaf():
left_( 0),
right_( 0),
x_( 0)
{
}

ConditionLeaf::ConditionLeaf( tinyxml2::XMLElement* e)
{
	tinyxml2::XMLElement* lparam =e->FirstChildElement( "lparam");
	tinyxml2::XMLElement* rparam =e->FirstChildElement( "rparam");

	std::string stype =XmlUtil::GetXmlAttrStr( lparam, "type", "const");
	if( stype == "const")
		left_ =new ConditionValueConst( lparam);
	else
		left_ =new ConditionValueObject( lparam);

	stype =XmlUtil::GetXmlAttrStr( rparam, "type", "const");
	if( stype == "const")
		right_ =new ConditionValueConst( rparam);
	else
		right_ =new ConditionValueObject( rparam);

	std::string copt =XmlUtil::GetXmlAttrStr( e, "operator", "great");
	if( copt == "between")
	{
		tinyxml2::XMLElement* xparam =e->FirstChildElement( "xparam");

		stype =XmlUtil::GetXmlAttrStr( xparam, "type", "const");
		if( stype == "const")
			x_ =new ConditionValueConst( xparam);
		else
			x_ =new ConditionValueObject( xparam);

		operator_ =CONDITION_OPERATOR_BETWEEN;
	}
	else if( copt == "great")
	{
		operator_ =CONDITION_OPERATOR_GREAT;
	}
	else if( copt == "egreat")
	{
		operator_ =CONDITION_OPERATOR_GREATEQUAL;
	}
	else if( copt == "less")
	{
		operator_ =CONDITION_OPERATOR_LESS;
	}
	else if( copt == "eless")
	{
		operator_ =CONDITION_OPERATOR_LESSEQUAL;
	}
	else if( copt == "equal")
	{
		operator_ =CONDITION_OPERATOR_EQUAL;
	}
	else if( copt == "nequal")
	{
		operator_ =CONDITION_OPERATOR_NOTEQUAL;
	}
	else
	{
		assert( false && "not supported operator type");
	}
}

ConditionLeaf::~ConditionLeaf()
{
	if( left_)
		delete left_;
	left_ =0;

	if( right_)
		delete right_;
	right_ =0;

	if( x_)
		delete x_;
	x_ =0;
}

bool ConditionLeaf::condition_calcuate( ConditionContext& context)
{
	PropertyOperatorValue lv =left_->get_conditionvalue( context);
	PropertyOperatorValue rv =right_->get_conditionvalue( context);

	if( operator_ == CONDITION_OPERATOR_BETWEEN)
	{
		PropertyOperatorValue xv =x_->get_conditionvalue( context);
		return xv > lv && xv < rv;
	}
	else if( operator_ == CONDITION_OPERATOR_GREAT)
		return lv > rv;
	else if( operator_ == CONDITION_OPERATOR_GREATEQUAL)
		return lv >= rv;
	else if( operator_ == CONDITION_OPERATOR_LESS)
		return lv < rv;
	else if( operator_ == CONDITION_OPERATOR_LESSEQUAL)
		return lv <= rv;
	else if( operator_ == CONDITION_OPERATOR_EQUAL)
		return lv == rv;
	else if( operator_ == CONDITION_OPERATOR_NOTEQUAL)
		return lv != rv;

	assert( false && "ConditionLeaf::condition_calcuate not support condition operator type");
	return false;
}
