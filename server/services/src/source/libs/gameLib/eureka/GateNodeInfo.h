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

#ifndef __GATENODEINFO_H__
#define __GATENODEINFO_H__

#include <string>
#include <cmsLib/core_type.h>
#include <cmsLib/redis/RedisClient.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

//tcp
#define GATE_LINKTYPE_TCP	1
//websocket
#define GATE_LINKTYPE_WS	2

//GateNodeInfo存放于redis
class GateNodeInfo : public IRedisSerializer
{
public:
	GateNodeInfo();
	GateNodeInfo(const ServiceNodeInfo& sinfo);
	virtual ~GateNodeInfo() {}

protected:
	//------------------implementation IRedisSerializer---------------------
	virtual bool to_json(std::string& val);
	virtual bool from_json(boost::json::value& root);

public:
	//服务id
	S_INT_64	iid;
	//node的ip地址
	std::string ip;
	//node的端口
	int			port;
	//连接类型 1:tcp 2:websocket
	S_INT_32	socket_type;
};

#endif //__GATENODEINFO_H__
