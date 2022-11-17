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

S_INT_64 CaseActionBase::get_params_int64(const char* key, S_INT_64 def)
{
	std::string val = get_params(key);

	if (val == "")
		return def;

	return (S_INT_64)strtoll(val.c_str(), 0, 10);
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

	action_begin_time_ =TestCaseWorkFlow::GetMillisecondTime();
}

void CaseActionBase::end_action()
{
	vuser_->set_callresult(error_code_);

	close_timeout();

	ca_status_ =CASTATUS_END;

	action_end_time_ =TestCaseWorkFlow::GetMillisecondTime();

	if (log_analysis())
	{
		//actionname, errorcode, time
		logInfo(out_runtime, "#analysis#{\"timestamp\":%lld,\"openid\":\"%s\",\"accountid\":\"%s\",\"action\":\"%s\",\"protoid\":\"0x%x\",\"delay\":%d,\"result\":%d}",
			action_end_time_, vuser_->user_openid_.c_str(), vuser_->account_id_.c_str(),
			get_action_name().c_str(), get_req_protoid(),
			(int)(action_end_time_ - action_begin_time_), error_code_);
	}

    error_code_ = 0;
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