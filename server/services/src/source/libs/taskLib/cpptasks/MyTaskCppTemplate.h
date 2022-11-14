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

#ifndef __MYTASKCPPTEMPLATE_H__
#define __MYTASKCPPTEMPLATE_H__

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/ITaskImplCpp.h>

class MyTaskCppTemplate : public ITaskImplCpp
{
public:
	MyTaskCppTemplate();
	virtual ~MyTaskCppTemplate() {}

	virtual bool can_gettask_check(ITaskContext* tc, CPPObjectiveParams* params);
	virtual S_INT_32 gettask_confirm(ITaskContext* tc, CPPObjectiveParams* params);
	virtual S_INT_32 submit_task(ITaskContext* tc, CPPObjectiveParams* params);
};

#endif //__MYTASKCPPTEMPLATE_H__