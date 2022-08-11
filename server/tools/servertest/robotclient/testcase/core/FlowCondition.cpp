#include "StdAfx.h"

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/Log.h>

#include "testcase/core/TestCaseWorkFlow.h"
#include "testcase/core/FlowCondition.h"
#include "testcase/core/FlowNode.h"
#include "testcase/VirtualUser.h"

FlowCondition::FlowCondition():
	next_flownode_id_( NO_INITVALUE),
	next_flownode_( 0),
	vuser_( 0),
	wf_( 0)
{
}

FlowCondition::~FlowCondition()
{
}

void FlowCondition::build_from_xml_sub(tinyxml2::XMLElement* f)
{
	next_flownode_id_ =NO_INITVALUE;
	next_flownode_ =0;

	next_flownode_id_ =XmlUtil::GetXmlAttrInt( f, "to", NO_INITVALUE);

	//or conditions
	tinyxml2::XMLElement* c =f->FirstChildElement();
	if( c != 0)
	{
		tinyxml2::XMLNode* pNode = c->FirstChild();
		condition_ = XmlUtil::GetXmlText(c);
		/*
		xercesc::DOMCDATASection* pNode =(xercesc::DOMCDATASection*)c->getFirstChild();
		if( pNode != 0)
		{
			const XMLCh* t1 =pNode->getTextContent();
			char* t2 =xercesc::XMLString::transcode( t1);
			condition_ =t2;
			xercesc::XMLString::release( &t2);
		}
		*/
	}
}

void FlowCondition::link_after_build( VirtualUser* v, TestCaseWorkFlow* f)
{
	vuser_ =v;
	wf_ =f;

	next_flownode_ =f->get_flownode_byid( next_flownode_id_);
}

void FlowCondition::init_before_start()
{
}

bool FlowCondition::is_condition_true()
{
	if( condition_.length() == 0)
		return false;

	return vuser_->run_script( condition_.c_str());
}
