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

#ifndef __SERVICENODEINFO_H__
#define __SERVICENODEINFO_H__

#include <string>
#include <boost/unordered_map.hpp>

#include <cmsLib/core_type.h>
#include <cmsLib/redis/RedisClient.h>
#include <gameLib/service_type.h>

class ServiceNodeInfo : public IRedisSerializer
{
public:
	ServiceNodeInfo();
	virtual ~ServiceNodeInfo() {}

	ServiceNodeInfo* clone();

	ServiceNodeInfo& operator = (const ServiceNodeInfo& v);
	void copy_to(PRO::ServerNode* pnode) const;
	void copy_from(const PRO::ServerNode* pnode);

	std::string get_extparam_bykey(const char* key);

	bool add_subscribe(NETSERVICE_TYPE st);
	bool add_router(NETSERVICE_TYPE st);

protected:
	//------------------implementation IRedisSerializer---------------------
	virtual bool to_json(std::string& val);
	virtual bool from_json(boost::json::value& root);

public:
	//服务id
	S_INT_64		iid;
	S_INT_64		token;
	NETSERVICE_TYPE	type;
	//扩展信息
	boost::unordered_map<std::string, std::string> extparams;

	//node的ip地址
	std::string ip;
	//node的端口
	int			port;
	//是否router服务
	bool		isrouter;

	//只在eureka节点使用的数据
	//状态,是否在线
	bool		isonline;
	//订阅的服务和负载均衡
	std::list<NETSERVICE_TYPE>	subscribes_;
	std::list<NETSERVICE_TYPE>	routers_;
};

#endif //__SERVICENODEINFO_H__
