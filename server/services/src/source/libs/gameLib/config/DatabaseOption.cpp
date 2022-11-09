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

#include <gameLib/config/DatabaseOption.h>

#include <cmsLib/util/XmlUtil.h>

using namespace config;

DatabaseOption::DatabaseOption()
{
}

void DatabaseOption::load_from_xml(tinyxml2::XMLElement* el)
{
	dbname_ = XmlUtil::GetXmlAttrStr(el, "db", "cms");
	dbip_ = XmlUtil::GetXmlAttrStr(el, "ip", "tcp://127.0.0.1:3306");
	dbuser_ = XmlUtil::GetXmlAttrStr(el, "user", "root");
	dbpwd_ = XmlUtil::GetXmlAttrStr(el, "pwd", "123456");
}
