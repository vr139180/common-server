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

ServiceNodeInfo::ServiceNodeInfo() :
	iid(0)
	, token(0)
	, type(NETSERVICE_TYPE::ERK_SERVICE_NONE)
	, port(0)
	, isrouter(false)
{
}

ServiceNodeInfo& ServiceNodeInfo::operator = (const ServiceNodeInfo& v)
{
	this->type = v.type;
	this->iid = v.iid;
	this->ip = v.ip;
	this->port = v.port;
	this->token = v.token;
	this->isrouter = v.isrouter;
	this->isonline = v.isonline;
	this->extparams = v.extparams;

	this->subscribes_ = v.subscribes_;
	this->routers_ = v.routers_;

	return *this;
}

void ServiceNodeInfo::copy_to(PRO::ServerNode* pnode) const
{
	pnode->set_iid(this->iid);
	pnode->set_token(this->token);
	pnode->set_ip(ip.c_str());
	pnode->set_port(port);
	pnode->set_isrouter(isrouter);
	pnode->set_isonline(isrouter);
	
	for (boost::unordered_map<std::string, std::string>::const_iterator iter = extparams.cbegin();
		iter != extparams.cend(); ++iter)
	{
		google::protobuf::Map<std::string, std::string>& ext = *(pnode->mutable_exts());
		ext[iter->first] = iter->second;
	}

	for (std::list<NETSERVICE_TYPE>::const_iterator iter = subscribes_.begin(); iter != subscribes_.end(); ++iter)
		pnode->add_subscribes((S_INT_64)(*iter));
	for (std::list<NETSERVICE_TYPE>::const_iterator iter = routers_.begin(); iter != routers_.end(); ++iter)
		pnode->add_routers((S_INT_64)(*iter));
}

void ServiceNodeInfo::copy_from(const PRO::ServerNode* pnode)
{
	iid = pnode->iid();
	token = pnode->token();
	ip = pnode->ip().c_str();
	port = pnode->port();
	isrouter = pnode->isrouter();
	isonline = pnode->isonline();

	const google::protobuf::Map<std::string, std::string>& kvs = pnode->exts();
	for (google::protobuf::Map<std::string, std::string>::const_iterator xiter = kvs.cbegin();
		xiter != kvs.cend(); ++xiter)
	{
		extparams[xiter->first] = xiter->second;
	}

	for (int ii = 0; ii < pnode->subscribes_size(); ++ii)
		subscribes_.push_back((NETSERVICE_TYPE)pnode->subscribes(ii));
	for (int ii = 0; ii < pnode->routers_size(); ++ii)
		routers_.push_back((NETSERVICE_TYPE)pnode->routers(ii));
}

ServiceNodeInfo* ServiceNodeInfo::clone()
{
	ServiceNodeInfo* r = new ServiceNodeInfo();
	*r = (*this);
	return r;
}

std::string ServiceNodeInfo::get_extparam_bykey(const char* key) const
{
	boost::unordered_map<std::string, std::string>::const_iterator fiter = extparams.find(key);
	if (fiter == extparams.cend())
		return "";
	return fiter->second;
}

bool ServiceNodeInfo::add_subscribe(NETSERVICE_TYPE st)
{
	std::list<NETSERVICE_TYPE>::iterator fiter = std::find(subscribes_.begin(), subscribes_.end(), st);
	if (fiter != subscribes_.end())
		return false;

	subscribes_.push_back(st);
	return true;
}

bool ServiceNodeInfo::add_router(NETSERVICE_TYPE st)
{
	std::list<NETSERVICE_TYPE>::iterator fiter = std::find(routers_.begin(), routers_.end(), st);
	if (fiter != routers_.end())
		return false;

	routers_.push_back(st);
	return true;
}

bool ServiceNodeInfo::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", iid},
		{"type", (int)type},
		{"ip", ip},
		{"port", port},
		{"token", token},
		{"isrouter", isrouter?1:0}
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
	this->isrouter = JSONUtil::get_value<int>(obj, "isrouter", 0) != 0;

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
