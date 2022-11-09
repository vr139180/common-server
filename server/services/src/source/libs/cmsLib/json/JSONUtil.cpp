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

#include "cmsLib/json/JSONUtil.h"

#if defined(EW_PLATFORM_LINUX)
#include <boost/json/src.hpp>
#endif

S_INT_64 JSONUtil::get_int64(boost::json::object& obj, const char* name, S_INT_64 def)
{
	try {
		boost::json::value& val = obj.at(name);
		return (S_INT_64)val.as_int64();
	}
	catch (...) {
		return def;
	}
}

std::string JSONUtil::get_string(boost::json::object& obj, const char* name, const char* def)
{
	try {
		boost::json::value& val = obj.at(name);
		return val.as_string().c_str();
	}
	catch (...) {
		return def;
	}
}

double JSONUtil::get_double(boost::json::object& obj, const char* name, double def)
{
	try {
		boost::json::value& val = obj.at(name);
		return val.as_double();
	}
	catch (...) {
		return def;
	}
}

S_INT_64 JSONUtil::str_2_int64(const char* val)
{
	return atol(val);
}