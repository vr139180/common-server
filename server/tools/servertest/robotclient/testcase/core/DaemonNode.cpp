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

#include "testcase/core/DaemonNode.h"

#include <cmsLib/core_type.h>
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

void DaemonNode::handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle)
{
}
