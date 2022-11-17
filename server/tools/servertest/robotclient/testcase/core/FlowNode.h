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

#ifndef __FLOWNODE_H__
#define __FLOWNODE_H__

#include <cmsLib/util/XmlUtil.h>
#include "testcase/core/CaseActionBase.h"
#include "testcase/core/FlowCondition.h"

#include <list>
#include <vector>

class VirtualUser;
class TestCaseWorkFlow;

class FlowNode
{
public:
	typedef enum enum_FLOWNODE_STATUS
	{
		FNSTATUS_IDLE =0,
		FNSTATUS_RUNNING,
		FNSTATUS_BEFOREEND,
		FNSTATUS_END
	}FLOWNODE_STATUS;

public:
	FlowNode( int id);
	virtual ~FlowNode();

	void load_flownode( tinyxml2::XMLElement* s);
	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f);

	int get_iid(){ return iid_;}
	const char* get_name(){ return name_.c_str();}

	void recv_protocol(S_UINT_16 proid, BasicProtocol *p, bool& handle);

public:
	void init_before_start();

	void start_flow();

	void update( ULONGLONG now);

	void fn_end(){ fn_status_ =FNSTATUS_END;}
	bool is_idle(){ return fn_status_ == FNSTATUS_IDLE;}
	bool is_end(){ return fn_status_ == FNSTATUS_END;}

protected:
	bool is_allaction_end();
	void flownode_will_end();

protected:
	FLOWNODE_STATUS		fn_status_;

protected:
	int iid_;
	std::string name_;
	std::string extra_info_;

	std::list<CaseActionBase*>	actions_;
	std::list<FlowCondition*>	conditions_;

	int					def_next_node_id_;
	FlowNode*			def_next_flownode_;

	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
};

#endif	//__FLOWNODE_H__
