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

#ifndef __ITASKRESTRIGGER_H__
#define __ITASKRESTRIGGER_H__

#include <taskLib/task_const.h>

//外部系统定义资源触发
class ITaskResTrigger
{
public:
	virtual void on_roleinfo_change() = 0;
	virtual void on_bag_change() = 0;
	virtual void on_building_change() = 0;
};

#endif //__ITASKRESTRIGGER_H__
