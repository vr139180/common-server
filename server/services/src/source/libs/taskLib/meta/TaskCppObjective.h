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

#ifndef __TASKCPPOBJECTIVE_H__
#define __TASKCPPOBJECTIVE_H__

#include <vector>
#include <string>
#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/util/XmlUtil.h>

#include <taskLib/task_const.h>

class CPPObjectiveParams
{
protected:
	class CPPParamValue
	{
	public:
		CPPParamValue(const char* v);
		CPPParamValue();

		int intval();
		S_INT_64 int64val();
		const char* strval();

		void setval(const char* v);

	private:
		std::string val;
	};

protected:
	CPPObjectiveParams();

public:
	static CPPObjectiveParams* build_cppparams(tinyxml2::XMLElement* e);
	static CPPObjectiveParams* build_empty() { return new CPPObjectiveParams(); }

	//index 1~≤Œ ˝Œª÷√
	bool intval_param(const char* key, int& val);
	bool int64val_param(const char* key, S_INT_64& val);
	bool strval_param(const char* key, std::string& val);

	int size() { return (int)params_.size(); }

protected:
	CPPParamValue* get_param( const char* key);

private:
	boost::unordered_map<std::string, CPPParamValue> params_;
};

#endif //__TASKCPPOBJECTIVE_H__
