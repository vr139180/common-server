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

#ifndef __ROUTERCONFIG_H__
#define __ROUTERCONFIG_H__

#include <string>
#include <gameLib/config/RedisOption.h>

class RouterConfig
{
public:
	RouterConfig(){}

public:
	//现成一次tick循环次数
	int loopnum_;
	//虚拟节点数量
	int vnode_;
};

#endif //__ROUTERCONFIG_H__
