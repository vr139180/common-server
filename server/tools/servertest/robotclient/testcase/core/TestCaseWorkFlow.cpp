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

#include "testcase/core/TestCaseWorkFlow.h"

#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/Log.h>
#include <gameLib/protobuf/Proto_all.h>

#include "TestCase.h"
#include "testcase/VirtualUser.h"

#include <string>

USE_PROTOCOL_NAMESPACE;

TestCaseWorkFlow::TestCaseWorkFlow():
	vuser_( 0),
	case_( 0),
	start_node_( 0),
	cur_node_( 0),
	wf_status_( WFSTATUS_NONE)
{
}

TestCaseWorkFlow::~TestCaseWorkFlow()
{
	std::map<int,FlowNode*>::iterator iter =flownodes_.begin(), eiter =flownodes_.end();
	for( ; iter != eiter; ++iter)
		delete iter->second;
	flownodes_.clear();
}

void TestCaseWorkFlow::init( TestCase* t, VirtualUser* v)
{
	this->vuser_ =v;
	this->case_ =t;
}

ULONGLONG TestCaseWorkFlow::GetMillisecondTime()
{
	FILETIME stime={0};
	GetSystemTimeAsFileTime( &stime);
	ULONGLONG ll =*((PULONGLONG)&stime);
	return ll/10000;
}

bool TestCaseWorkFlow::send_gts_protocol( BasicProtocol *p)
{
	if( p == 0)
		return false;

	case_->send_gts_protocol( p);
	delete p;

	return true;
}

void TestCaseWorkFlow::prepare( const char* xml)
{
	for( std::map<int,FlowNode*>::iterator iter =flownodes_.begin(); iter != flownodes_.end(); ++iter)
		delete iter->second;
	flownodes_.clear();
	start_node_ =0;
	cur_node_ =0;

	build_testcase( xml);

	//link all
	for( std::map<int,FlowNode*>::iterator iter =flownodes_.begin(); iter != flownodes_.end(); ++iter)
	{
		FlowNode* pn =iter->second;
		pn->link_after_build( vuser_, this);
	}

	daemon_ctrl_.link_after_build( vuser_, this);
}

void TestCaseWorkFlow::build_testcase( const char* xml)
{
	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(xml, doc))
		return;

	tinyxml2::XMLElement* root = doc.RootElement();

	for(tinyxml2::XMLElement* s = root->FirstChildElement("s"); s != 0; s = s->NextSiblingElement("s"))
	{
		int id = XmlUtil::GetXmlAttrInt(s, "id", -1);
		assert(id != -1);

		FlowNode* nod = new FlowNode(id);
		flownodes_[nod->get_iid()] = nod;

		nod->load_flownode(s);
	}

	int snode = XmlUtil::GetXmlAttrInt(root, "s", NO_INITVALUE);
	if (snode != NO_INITVALUE)
		start_node_ = get_flownode_byid(snode);
}

FlowNode* TestCaseWorkFlow::get_flownode_byid( int id)
{
	std::map<int,FlowNode*>::iterator fiter =flownodes_.find( id);
	if( fiter == flownodes_.end())
		return 0;

	return fiter->second;
}

void TestCaseWorkFlow::recv_protocol(S_UINT_16 proid, BasicProtocol *p)
{
	assert( !is_idle());

	if( is_stop())
		return;

	bool handle =false;

	cur_node_->recv_protocol( proid, p, handle);

	if( !handle)
		daemon_ctrl_.handle_message( proid, p, handle);
}

void TestCaseWorkFlow::start_workflow()
{
	if( !is_idle())
		return;

	vuser_->init_before_start();

	cur_node_ =0;

	daemon_ctrl_.init_before_start();

	for( std::map<int,FlowNode*>::iterator iter =flownodes_.begin(); iter != flownodes_.end(); ++iter)
	{
		FlowNode* fn =iter->second;
		fn->init_before_start();
	}

	if( start_node_ == 0)
	{
		wf_status_ =WFSTATUS_STOP;
		return;
	}

	cur_node_ =start_node_;

	wf_status_ =WFSTATUS_RUNNING;
}

void TestCaseWorkFlow::goto_next_flownode( FlowNode* nextfn)
{
	if( nextfn == 0)
	{
#ifdef _DEBUG
		logWarn( out_runtime, ">>>>>>>>>> next node:none");
#endif
	}

	if( !is_running())
		return;

	assert( cur_node_->is_end());
	
	cur_node_ =nextfn;

	if( cur_node_ != 0)
		cur_node_->init_before_start();
}

void TestCaseWorkFlow::update( ULONGLONG now)
{
	if( is_idle())
		start_workflow();

	if( is_running())
	{
		if( cur_node_ != 0)
			cur_node_->update( now);

		if( cur_node_ == 0)
			wf_status_ =WFSTATUS_STOP;
	}
}