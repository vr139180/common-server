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

#include "gameLib/config/RedisOption.h"

#include <cmsLib/util/XmlUtil.h>

using namespace config;

RedisOption::RedisOption()
{
}

void RedisOption::load_from_xml(tinyxml2::XMLElement* el)
{
	name_ = XmlUtil::GetXmlAttrStr(el, "name");
	ip_ = XmlUtil::GetXmlAttrStr(el, "ip", "127.0.0.1");
	port_ = XmlUtil::GetXmlAttrInt(el, "port", 6379);
	auth_ = XmlUtil::GetXmlAttrStr(el, "auth", "");
	db_ = XmlUtil::GetXmlAttrInt(el, "db", 0);
	socket_timeout_ = XmlUtil::GetXmlAttrInt(el, "socket_timeout", 100);
}
