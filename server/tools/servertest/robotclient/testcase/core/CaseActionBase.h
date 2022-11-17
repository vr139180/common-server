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

#ifndef __CASEACTIONBASE_H__
#define __CASEACTIONBASE_H__

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <cmsLib/util/XmlUtil.h>
#include <string>
#include <map>

class VirtualUser;
class TestCaseWorkFlow;
class FlowNode;

class CaseActionBase
{
public:
	typedef enum enum_CA_STATUS
	{
		CASTATUS_IDLE =0,
		CASTATUS_RUNNING,
		CASTATUS_END,
	}CA_STATUS;

public:
	static CaseActionBase* build_from_xml( tinyxml2::XMLElement* act);
	static std::string get_done_name(){ return "none";}

public:
	CaseActionBase();
	virtual ~CaseActionBase();

	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f, FlowNode* n);

public:
	virtual bool log_analysis() { return true; }
	virtual std::string get_action_name() = 0;
	virtual S_UINT_32 get_req_protoid() = 0;

	virtual void init_before_start();

	virtual void update( ULONGLONG now);

	virtual void handle_message( S_UINT_16 proid, BasicProtocol* pro, bool& handle){}

	bool is_idle(){ return ca_status_ == CASTATUS_IDLE;}
	bool is_running(){ return ca_status_ == CASTATUS_RUNNING;}
	bool is_end(){ return ca_status_ == CASTATUS_END;}

	std::string get_params( const char* key);
	int get_params_int( const char* key, int def =0);
	S_INT_64 get_params_int64(const char* key, S_INT_64 def = 0);

	int get_doing_time(){
		return (int)( action_end_time_ - action_begin_time_);
	}

protected:
	virtual void start_action();
	virtual void end_action();

	virtual void update_do( ULONGLONG now){}

	void open_timeout( int ot =10000);	//default 10s
	void close_timeout();
	virtual void time_out(){}

protected:
	virtual void build_from_xml_sub(tinyxml2::XMLElement* act);

protected:
	CA_STATUS	ca_status_;

	ULONGLONG	action_begin_time_;
	ULONGLONG	action_end_time_;

	std::map< std::string, std::string >	params_;

	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
    FlowNode*			node_;

    int                 error_code_;

private:
	bool				timeout_support_;
	int					timeout_value_;
	ULONGLONG			timeout_begin_time_;
};

#endif	//__CASEACTIONBASE_H__
