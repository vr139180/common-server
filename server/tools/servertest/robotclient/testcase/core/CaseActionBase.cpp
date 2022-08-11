#include "StdAfx.h"

#include "testcase/core/CaseActionBase.h"

#include <cmsLib/Log.h>

#include "testcase/VirtualUser.h"
#include "testcase/core/TestCaseWorkFlow.h"
#include "testcase/core/FlowNode.h"


CaseActionBase::CaseActionBase():
	vuser_( 0),
	wf_( 0),
	node_( 0),
	ca_status_( CASTATUS_IDLE),
	timeout_support_( 0)
{
}

CaseActionBase::~CaseActionBase()
{
	params_.clear();
}

void CaseActionBase::build_from_xml_sub(tinyxml2::XMLElement* act)
{
	params_.clear();

	for( tinyxml2::XMLElement* pm = act->FirstChildElement("pm"); pm != 0; pm = pm->NextSiblingElement("pm"))
	{
		std::string n =XmlUtil::GetXmlAttrStr( pm, "n");
		if( n == "")
			continue;

		std::string v =XmlUtil::GetXmlAttrStr( pm, "v");
		if( v == "")
			continue;

		params_[n] =v;
	}
}

std::string CaseActionBase::get_params( const char* key)
{
	std::map< std::string, std::string >::iterator fiter =params_.find( key);
	if( fiter == params_.end())
		return "";

	return fiter->second;
}

int CaseActionBase::get_params_int( const char* key, int def)
{
	std::string val =get_params( key);

	if( val == "")
		return def;

	return atoi( val.c_str());
}

void CaseActionBase::link_after_build( VirtualUser* v, TestCaseWorkFlow* f, FlowNode* n)
{
	vuser_ =v;
	wf_ =f;
	node_ =n;
}

void CaseActionBase::init_before_start()
{
	ca_status_ =CASTATUS_IDLE;
    error_code_ = NO_INITVALUE;
	timeout_support_ =false;
}

void CaseActionBase::update( ULONGLONG now)
{
	if( is_end())
		return;

	if( is_idle())
		start_action();

	if( is_running())
	{
		if( timeout_support_)
		{
			if( timeout_begin_time_ + timeout_value_ <= now)
			{
				time_out();
				end_action();
				return;
			}
		}

		update_do( now);
	}
}

void CaseActionBase::start_action()
{
	ca_status_ =CASTATUS_RUNNING;

    error_code_ = 0;
	error_step_ = "";

	action_begin_time_ =TestCaseWorkFlow::GetMillisecondTime();
}

void CaseActionBase::end_action()
{
	close_timeout();

	ca_status_ =CASTATUS_END;

	action_end_time_ =TestCaseWorkFlow::GetMillisecondTime();

//#ifdef _DEBUG
	logInfo( out_runtime, "................. node:%s[%d] done, user id: %d, error code: %d, error step: %s, time: %d ms", 
		node_->get_name(), node_->get_iid(), vuser_->userid_, error_code_, error_step_, (int)(action_end_time_ - action_begin_time_));
//#endif

    error_code_ = 0;
	error_step_ = "";
}

void CaseActionBase::open_timeout( int ot)
{
	timeout_support_ =true;
	timeout_value_ =ot;
	timeout_begin_time_ =TestCaseWorkFlow::GetMillisecondTime();
}

void CaseActionBase::close_timeout()
{
	timeout_support_ =false;
}