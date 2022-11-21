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

#ifndef __GATECONFIG_H__
#define __GATECONFIG_H__

#include <string>
#include <cmsLib/core_type.h>

class GateConfig
{
public:
	GateConfig():gate_type_(NetSessionType::NSType_TCP){}

	void set_gate_type(const std::string& t) {
		if (t == "ws")
			gate_type_ = NetSessionType::NSType_WebSocket;
		else
			gate_type_ = NetSessionType::NSType_TCP;
	}

public:
	//现成一次tick循环次数
	int loopnum_;
	//gate的类型 tcp or websocket
	NetSessionType	gate_type_;
};

#endif //__GATECONFIG_H__
