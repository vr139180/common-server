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

#include "testcase/core/UtilServiceAction.h"
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include "TestCase.h"

USE_PROTOCOL_NAMESPACE;

//-------------------------------------EndTestAction--------------------------
EndTestAction::EndTestAction() :CaseActionBase()
{
}

void EndTestAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub(act);
}

void EndTestAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void EndTestAction::start_action()
{
	CaseActionBase::start_action();

	end_action();

	wf_->get_testcase()->end_case();
}

void EndTestAction::end_action()
{
	CaseActionBase::end_action();
}

//-------------------------------------IdleAction--------------------------
IdleAction::IdleAction():CaseActionBase()
{
}

void IdleAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub( act);

	idle_time_ =get_params_int( "step", 500);
}

void IdleAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void IdleAction::start_action()
{
	CaseActionBase::start_action();

	idle_begin_time_ =TestCaseWorkFlow::GetMillisecondTime();
}

void IdleAction::end_action()
{
	CaseActionBase::end_action();
}

void IdleAction::update_do( ULONGLONG now)
{
	if( idle_begin_time_ + idle_time_ < now)
		end_action();
}

void IdleAction::handle_message( BasicProtocol* pro, bool& handle)
{
}


//-------------------------------------DelayStartAction--------------------------
DelayStartAction::DelayStartAction() :CaseActionBase()
{
}

void DelayStartAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub(act);

	user_range_delay_ = get_params_int("range", 200);
	if (user_range_delay_ < 1)
		user_range_delay_ = 1;
	idle_time_ = get_params_int("delay", 1000);
}

void DelayStartAction::init_before_start()
{
	CaseActionBase::init_before_start();

	idle_time_ = (vuser_->init_iid_ / user_range_delay_)*idle_time_;
}

void DelayStartAction::start_action()
{
	CaseActionBase::start_action();
	idle_begin_time_ = TestCaseWorkFlow::GetMillisecondTime();
}

void DelayStartAction::end_action()
{
	CaseActionBase::end_action();
}

void DelayStartAction::update_do(ULONGLONG now)
{
	if (idle_begin_time_ + idle_time_ < now)
		end_action();
}

//-------------------------------------CallFailAction--------------------------
CallFailAction::CallFailAction() :CaseActionBase()
{
}

void CallFailAction::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	CaseActionBase::build_from_xml_sub(act);
}

void CallFailAction::init_before_start()
{
	CaseActionBase::init_before_start();
}

void CallFailAction::start_action()
{
	CaseActionBase::start_action();

	end_action();
}

void CallFailAction::end_action()
{
	CaseActionBase::end_action();
}