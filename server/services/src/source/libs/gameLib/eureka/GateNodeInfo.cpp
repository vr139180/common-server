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

#include <gameLib/eureka/GateNodeInfo.h>

#include <cmsLib/json/JSONUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/global_const.h>

GateNodeInfo::GateNodeInfo():
	iid(0),port(0), socket_type(1)
{
}

GateNodeInfo::GateNodeInfo(const ServiceNodeInfo& sinfo)
{
	iid = sinfo.iid;
	ip = sinfo.ip;
	port = sinfo.port;
	socket_type = ShareUtil::atoi(sinfo.get_extparam_bykey(GATE_INSTANCE_EXTPARAM).c_str());
}

bool GateNodeInfo::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", iid},
		{"ip", ip},
		{"port", port},
		{"type", socket_type}
	};

	val = boost::json::serialize(json);

	return true;
}

bool GateNodeInfo::from_json(boost::json::value& root)
{
	boost::json::object& obj = root.as_object();

	this->iid = JSONUtil::get_int64(obj, "iid", 0);
	this->socket_type = JSONUtil::get_value<int>(obj, "type", -1);
	if (this->socket_type == -1)
		return false;
	this->ip = JSONUtil::get_string(obj, "ip", "");
	this->port = JSONUtil::get_value<int>(obj, "port", 0);

	return true;
}
