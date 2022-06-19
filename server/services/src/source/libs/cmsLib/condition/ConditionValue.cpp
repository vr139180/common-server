#include "cmsLib/condition/ConditionValue.h"

#include "cmsLib/condition/ConditionContext.h"
#include "cmsLib/util/XmlUtil.h"

//--------------------------------------------ConditionValueConst-------------------------------------------------
ConditionValueConst::ConditionValueConst( int val)
{
	const_value_.set_int( val);
}

ConditionValueConst::ConditionValueConst( float val)
{
	const_value_.set_float( val);
}

ConditionValueConst::ConditionValueConst( const char* val)
{
	const_value_.set_str( val);
}

ConditionValueConst::ConditionValueConst( tinyxml2::XMLElement* e)
{
	std::string ctype =XmlUtil::GetXmlAttrStr( e, "consttype", "int");
	if( ctype == "int")
	{
		int v =XmlUtil::GetXmlAttrInt( e, "constvalue", 0);
		const_value_.set_int( v);
	}
	else if( ctype == "float")
	{
		float v =XmlUtil::GetXmlAttrFloat( e, "constvalue", 0);
		const_value_.set_float( v);
	}
	else if( ctype == "string")
	{
		std::string v =XmlUtil::GetXmlAttrStr( e, "constvalue", "");
		const_value_.set_str( v.c_str());
	}
	else
	{
		assert( false && "not supported condition const value");
	}
}

PropertyOperatorValue ConditionValueConst::get_conditionvalue( ConditionContext& context)
{
	return const_value_;
}


//-------------------------------------------ConditionValueObject------------------------------------------------
ConditionValueObject::ConditionValueObject( const char* objname, const char* propname):
objname_( objname),
propname_( propname)
{
}

ConditionValueObject::ConditionValueObject( tinyxml2::XMLElement* e)
{
	objname_ =XmlUtil::GetXmlAttrStr( e, "objectname", "");
	propname_ =XmlUtil::GetXmlAttrStr( e, "propertyname", "");
	assert( objname_ != "" && propname_ != "");
}

PropertyOperatorValue ConditionValueObject::get_conditionvalue( ConditionContext& context)
{
	IPropertyOperator* obj =context.get_object( objname_.c_str());
	assert( obj != 0);

	return obj->get_property( propname_.c_str());
}
