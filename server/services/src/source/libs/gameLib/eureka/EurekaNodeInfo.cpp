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

#include "gameLib/eureka/EurekaNodeInfo.h"

#include <cmsLib/json/JSONUtil.h>

EurekaNodeInfo::EurekaNodeInfo():iid( 0)
,port( 0)
,token( 0)
{
}

EurekaNodeInfo& EurekaNodeInfo::operator = (const EurekaNodeInfo& v)
{
	this->iid = v.iid;
	this->ip = v.ip;
	this->port = v.port;
	this->token = v.token;

	return *this;
}

EurekaNodeInfo* EurekaNodeInfo::clone()
{
	EurekaNodeInfo* r = new EurekaNodeInfo();
	*r = (*this);
	return r;
}

bool EurekaNodeInfo::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", iid},
		{"ip", ip},
		{"port", port},
		{"token", token}
	};

	val = boost::json::serialize(json);

	return true;
}

bool EurekaNodeInfo::from_json(boost::json::value& root)
{
	boost::json::object obj = root.as_object();

	this->iid = JSONUtil::get_int64( obj, "iid", 0);
	this->ip =JSONUtil::get_string( obj, "ip", "");
	this->port = JSONUtil::get_value<int>(obj, "port", 0);
	this->token = JSONUtil::get_int64(obj, "token", 0);

	return true;
}
