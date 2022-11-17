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

#include <cmsLib/core_type.h>
#include <cmsLib/util/ShareUtil.h>

#include "testcase/core/FlowNode.h"
#include "testcase/core/TestCaseWorkFlow.h"
#include "testcase/VirtualUser.h"

FlowNode::FlowNode( int id):
	iid_( id),
	vuser_( 0),
	wf_( 0),
	def_next_flownode_( 0),
	fn_status_( FNSTATUS_IDLE)
{
}

FlowNode::~FlowNode()
{
	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
		delete (*iter);
	actions_.clear();

	for( std::list<FlowCondition*>::iterator iter =conditions_.begin(); iter != conditions_.end(); ++iter)
		delete (*iter);
	conditions_.clear();
}

void FlowNode::load_flownode(tinyxml2::XMLElement* s)
{
	def_next_node_id_ =NO_INITVALUE;
	def_next_flownode_ =0;

	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
		delete (*iter);
	actions_.clear();

	for( std::list<FlowCondition*>::iterator iter =conditions_.begin(); iter != conditions_.end(); ++iter)
		delete (*iter);
	conditions_.clear();

	name_ =XmlUtil::GetXmlAttrStr( s, "n");

	//build from xml
	tinyxml2::XMLElement* acts =s->FirstChildElement( "acts");
	if( acts != 0)
	{
		for( tinyxml2::XMLElement* act = acts->FirstChildElement("act"); act != 0; act = act->NextSiblingElement("act"))
		{
			CaseActionBase* ca =CaseActionBase::build_from_xml( act);
			actions_.push_back( ca);
		}
	}

	tinyxml2::XMLElement* fs = s->FirstChildElement("fs");
	if( fs != 0)
	{
		def_next_node_id_ =XmlUtil::GetXmlAttrInt( fs, "def", NO_INITVALUE);

		for(tinyxml2::XMLElement* f = fs->FirstChildElement("f"); f != 0; f = f->NextSiblingElement("f"))
		{
			FlowCondition* fc =new FlowCondition();
			fc->build_from_xml_sub( f);

			conditions_.push_back( fc);
		}
	}
}

void FlowNode::link_after_build( VirtualUser* v, TestCaseWorkFlow* f)
{
	vuser_ =v;
	wf_ =f;

	def_next_flownode_ =wf_->get_flownode_byid( def_next_node_id_);

	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
	{
		CaseActionBase* act =(*iter);
		act->link_after_build( v, f, this);
	}

	for( std::list<FlowCondition*>::iterator iter =conditions_.begin(); iter != conditions_.end(); ++iter)
	{
		FlowCondition* cond =(*iter);
		cond->link_after_build( v, f);
	}
}

void FlowNode::recv_protocol(S_UINT_16 proid, BasicProtocol *p, bool& handle)
{
	if( is_end())
		return;

	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
	{
		CaseActionBase* act =(*iter);
		if( act->is_end())
			continue;

		act->handle_message( proid, p, handle);

		if( handle)
			break;
	}
}

void FlowNode::init_before_start()
{
	fn_status_ =FNSTATUS_IDLE;

	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
	{
		CaseActionBase* act =(*iter);
		act->init_before_start();
	}

	for( std::list<FlowCondition*>::iterator iter =conditions_.begin(); iter != conditions_.end(); ++iter)
	{
		FlowCondition* cond =(*iter);
		cond->init_before_start();
	}
}

void FlowNode::update( ULONGLONG now)
{
	if( is_idle())
		start_flow();

	if( fn_status_ == FNSTATUS_RUNNING)
	{
		if( is_allaction_end())
			fn_status_ =FNSTATUS_BEFOREEND;
		else
		{
			for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
			{
				CaseActionBase* act =(*iter);
				act->update( now);
			}
		}
	}
	else if( fn_status_ == FNSTATUS_BEFOREEND)
	{
		flownode_will_end();
	}
}

void FlowNode::start_flow()
{
	if( !is_idle())
		return;

	fn_status_ =FNSTATUS_RUNNING;
}

bool FlowNode::is_allaction_end()
{
	for( std::list<CaseActionBase*>::iterator iter =actions_.begin(); iter != actions_.end(); ++iter)
	{
		CaseActionBase* act =(*iter);
		if( !act->is_end())
			return false;
	}

	return true;
}

void FlowNode::flownode_will_end()
{
	fn_status_ =FNSTATUS_END;

	//decide which flow node is next node
	//check condition
	std::vector<FlowCondition*> gocond;
	for( std::list<FlowCondition*>::iterator iter =conditions_.begin(); iter != conditions_.end(); ++iter)
	{
		FlowCondition* cond =(*iter);
		if( cond->is_condition_true())
			gocond.push_back( cond);
	}

	if( gocond.size() == 0)
	{
		wf_->goto_next_flownode( def_next_flownode_);
	}
	else
	{
		int rnum =(int)gocond.size();
		rnum =std::rand()% rnum;

		FlowCondition* nf =gocond[rnum];
		wf_->goto_next_flownode( nf->get_next_flownode());
	}
}