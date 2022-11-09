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

#include "gameLib/eureka/ServiceNodeInfo.h"

#include <cmsLib/json/JSONUtil.h>

ServiceNodeInfo::ServiceNodeInfo() :iid(0)
, port(0)
, token(0)
, type(NETSERVICE_TYPE::ERK_SERVICE_NONE)
{
}

ServiceNodeInfo& ServiceNodeInfo::operator = (const ServiceNodeInfo& v)
{
	this->type = v.type;
	this->iid = v.iid;
	this->ip = v.ip;
	this->port = v.port;
	this->token = v.token;

	return *this;
}

ServiceNodeInfo* ServiceNodeInfo::clone()
{
	ServiceNodeInfo* r = new ServiceNodeInfo();
	*r = (*this);
	return r;
}

std::string ServiceNodeInfo::get_extparam_bykey(const char* key)
{
	boost::unordered_map<std::string, std::string>::iterator fiter = extparams.find(key);
	if (fiter == extparams.end())
		return "";
	return fiter->second;
}

bool ServiceNodeInfo::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", iid},
		{"type", (int)type},
		{"ip", ip},
		{"port", port},
		{"token", token}
	};
	
	if (extparams.size() > 0)
	{
		boost::json::object& exts = json["exts"].emplace_object();		
		for (boost::unordered_map<std::string, std::string>::const_iterator iter = extparams.cbegin();
			iter != extparams.cend(); ++iter)
		{
			exts[iter->first] = iter->second;
		}
	}

	val = boost::json::serialize(json);

	return true;
}

bool ServiceNodeInfo::from_json(boost::json::value& root)
{
	boost::json::object& obj = root.as_object();

	this->iid = JSONUtil::get_int64(obj, "iid", 0);
	int v = JSONUtil::get_value<int>(obj, "type", 0);
	if (v == 0)
		return false;
	this->type = (NETSERVICE_TYPE)v;
	this->ip = JSONUtil::get_string(obj, "ip", "");
	this->port = JSONUtil::get_value<int>(obj, "port", 0);
	this->token = JSONUtil::get_int64(obj, "token", 0);

	try {
		if (obj.contains("exts"))
		{
			boost::json::object& extobj = obj.at("exts").as_object();
			for (boost::json::object::iterator iter = extobj.begin(); iter != extobj.end(); ++iter)
			{
				std::string k = iter->key_c_str();
				std::string v = iter->value().as_string().c_str();
				this->extparams[k] = v;
			}
		}
	}
	catch (...) {
		return false;
	}

	return true;
}
