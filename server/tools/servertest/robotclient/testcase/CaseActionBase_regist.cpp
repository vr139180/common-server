#include "StdAfx.h"

#include <cmsLib/util/ShareUtil.h>

#include "testcase/core/CaseActionBase.h"
#include "testcase/core/UtilServiceAction.h"

#include "testcase/BeforeServiceAction.h"

CaseActionBase* CaseActionBase::build_from_xml( tinyxml2::XMLElement* act)
{
	CaseActionBase* ret =0;

	std::string done =XmlUtil::GetXmlAttrStr( act, "done");

	if( done == LoginAction::get_done_name())			//login
		ret =new LoginAction();
	else if( done == LogoutAction::get_done_name())		//logout
		ret =new LogoutAction();
	else if( done == SelChrAction::get_done_name())		//selchr
		ret =new SelChrAction();
	else if( done == CreateChrAction::get_done_name())	//createchr
		ret =new CreateChrAction();
	else if( done == IdleAction::get_done_name())		//idle
		ret =new IdleAction();

	if( ret != 0)
		ret->build_from_xml_sub( act);

	return ret;
}
