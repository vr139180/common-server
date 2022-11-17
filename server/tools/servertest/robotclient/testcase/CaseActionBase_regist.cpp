// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "StdAfx.h"

#include <cmsLib/util/ShareUtil.h>

#include "testcase/core/CaseActionBase.h"
#include "testcase/core/UtilServiceAction.h"

#include "testcase/BeforeServiceAction.h"

CaseActionBase* CaseActionBase::build_from_xml( tinyxml2::XMLElement* act)
{
	CaseActionBase* ret =0;

	std::string done =XmlUtil::GetXmlAttrStr( act, "done");

	if (done == IdleAction::get_done_name())				//idle
		ret = new IdleAction();
	else if (done == DelayStartAction::get_done_name())		//delaystart
		ret = new DelayStartAction();
	else if (done == EndTestAction::get_done_name())		//endtest
		ret = new EndTestAction();
	else if (done == CallFailAction::get_done_name())		//callfail
		ret = new CallFailAction();
	else if (done == LoginAction::get_done_name())			//login
		ret =new LoginAction();
	else if( done == LogoutAction::get_done_name())			//logout
		ret =new LogoutAction();
	else if( done == SelChrAction::get_done_name())			//selchr
		ret =new SelChrAction();
	else if( done == CreateChrAction::get_done_name())		//createchr
		ret =new CreateChrAction();

	if( ret != 0)
		ret->build_from_xml_sub( act);

	return ret;
}
