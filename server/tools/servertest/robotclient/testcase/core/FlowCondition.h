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

#ifndef __FLOWCONDITION_H__
#define __FLOWCONDITION_H__

#include <string>
#include <map>
#include <list>

#include <cmsLib/util/XmlUtil.h>

class FlowNode;
class VirtualUser;
class TestCaseWorkFlow;

class FlowCondition
{
public:
	FlowCondition();
	virtual ~FlowCondition();

	void build_from_xml_sub( tinyxml2::XMLElement* f);
	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f);

	virtual void init_before_start();

	FlowNode* get_next_flownode(){ return next_flownode_;}

	bool is_condition_true();

protected:
	int			next_flownode_id_;
	FlowNode*	next_flownode_;

	std::string	condition_;

	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
};

#endif	//__FLOWCONDITION_H__
