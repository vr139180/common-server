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

#ifndef __JSONUTIL_H__
#define __JSONUTIL_H__

#include <string>
#include <boost/json.hpp>
#include <cmsLib/core_type.h>

class JSONUtil
{
private:
	JSONUtil() {}

public:
	static S_INT_64 get_int64(boost::json::object& obj, const char* name, S_INT_64 def = 0);

	template<typename T>
	static T get_value(boost::json::object& obj, const char* name, T def);

	static std::string get_string(boost::json::object& obj, const char* name, const char* def = "");
	static double get_double(boost::json::object& obj, const char* name, double def = 0.0);

	static S_INT_64 str_2_int64(const char* val);
};

template<typename T>
T JSONUtil::get_value(boost::json::object& obj, const char* name, T def)
{
	try {
		boost::json::value& val = obj.at(name);
		return val.to_number<T>();
	}
	catch (...) {
		return def;
	}
}

#endif //__JSONUTIL_H__