#include "cmsLib/condition/ConditionElement.h"

#include <string>
#include "cmsLib/util/XmlUtil.h"

#include "cmsLib/condition/ConditionLeaf.h"
#include "cmsLib/condition/ConditionNode.h"

ConditionElement* ConditionElement::construct_condition( tinyxml2::XMLElement* e)
{
	tinyxml2::XMLElement* root = e->FirstChildElement( "condition");
	if( root == 0)
		return 0;

	std::string tye =XmlUtil::GetXmlAttrStr( root, "type", "node");
	if( tye == "node")
		return new ConditionNode( root);
	else if( tye == "leaf")
		return new ConditionLeaf( root);

	return 0;
}
