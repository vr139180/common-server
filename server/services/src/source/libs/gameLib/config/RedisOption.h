﻿// Copyright 2021 common-server Authors
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

#ifndef __REDISOPTION_H__
#define __REDISOPTION_H__

#include <string>

namespace tinyxml2 {
	class XMLElement;
}

namespace config {
	class RedisOption
	{
	public:
		RedisOption();

		void load_from_xml(tinyxml2::XMLElement* el);

	public:
		std::string name_;

		//用户名口令
		std::string ip_;
		int port_;
		std::string auth_;
		int db_;
		int socket_timeout_;
	};
}

#endif	//__REDISOPTION_H__
