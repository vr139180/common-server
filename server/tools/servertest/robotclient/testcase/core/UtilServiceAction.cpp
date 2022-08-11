#include "StdAfx.h"

#include "testcase/core/UtilServiceAction.h"
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include "TestCase.h"

USE_PROTOCOL_NAMESPACE;

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
