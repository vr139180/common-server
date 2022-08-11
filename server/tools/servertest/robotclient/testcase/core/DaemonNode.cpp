#include "StdAfx.h"

#include "testcase/core/DaemonNode.h"

#include <cmsLib/prolib/core_type.h>
#include <cmsLib/util/XmlUtil.h>

#include "testcase/VirtualUser.h"
#include "testcase/core/TestCaseWorkFlow.h"

DaemonNode::DaemonNode():
	vuser_( 0),
	wf_( 0)
{
}

DaemonNode::~DaemonNode()
{
}

void DaemonNode::link_after_build( VirtualUser* v, TestCaseWorkFlow* f)
{
	vuser_ =v;
	wf_ =f;
}

void DaemonNode::init_before_start()
{
}

void DaemonNode::handle_message( S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
	/*
	if( pro->iid_ == SKIRMISH_INFO_NTF)
	{
		handle =true;

		Skirmish_Info_ntf* ntf =dynamic_cast<Skirmish_Info_ntf*>(pro);
        vuser_->skirmish_season_id_ = ntf->season_id_;
		vuser_->skirmish_id_ =ntf->matchmaking_id_;
		vuser_->skirmish_trainid_ =ntf->training_id_;
	}
	*/
}
