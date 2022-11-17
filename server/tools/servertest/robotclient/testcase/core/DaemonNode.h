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

#ifndef __DAEMONNODE_H__
#define __DAEMONNODE_H__

#include <cmsLib/core_type.h>
#include <cmsLib/util/XmlUtil.h>
#include <gameLib/protobuf/Proto_all.h>

#include <string>
#include <map>
#include <list>

class VirtualUser;
class TestCaseWorkFlow;

class DaemonNode
{
public:
	DaemonNode();
	virtual ~DaemonNode();

	void link_after_build( VirtualUser* v, TestCaseWorkFlow* f);

public:
	virtual void init_before_start();

	virtual void update( ULONGLONG now){}

	virtual void handle_message(S_UINT_16 proid, BasicProtocol* pro, bool& handle);

protected:
	virtual void update_do( ULONGLONG now){}

protected:
	VirtualUser*		vuser_;
	TestCaseWorkFlow*	wf_;
};

#endif	//__DAEMONNODE_H__
