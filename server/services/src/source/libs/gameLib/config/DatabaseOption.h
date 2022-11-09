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

#ifndef __DATABASEOPTION__H__
#define __DATABASEOPTION__H__

#include <string>

namespace tinyxml2 {
	class XMLElement;
}

namespace config {

	class DatabaseOption
	{
	public:
		DatabaseOption();

		void load_from_xml(tinyxml2::XMLElement* el);

	public:
		//用户名口令
		std::string dbuser_;
		std::string dbpwd_;
		//数据库名字
		std::string dbname_;
		std::string dbip_;
	};
}

#endif	//__DATABASEOPTION__H__
