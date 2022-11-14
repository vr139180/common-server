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

#ifndef __TASKFUNC_DEFINED_H__
#define __TASKFUNC_DEFINED_H__

#include <string>

namespace task
{


	//获取角色等级
	static const char* FN_get_role_level = "get_role_level";
	//获取背包中指定物品的数量
	static const char* FN_get_bag_itemnum = "get_bag_itemnum";
}

#endif //__TASKFUNC_DEFINED_H__
