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

#ifndef __PHPRESULT_H__
#define __PHPRESULT_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/json/JSONUtil.h>
#include <vector>

class PhpResult
{
private:
	PhpResult() {
		cur_svr_ = 0; error_code_= 0;
	}

public:
	static PhpResult* build_from_str(const char* str);

	~PhpResult();

	void get_ipinfo(std::string& ip, int& port);

	S_INT_64 get_accountid() { return account_id_; }
	void set_accountid(S_INT_64 acc) { account_id_ = acc; }

protected:
	void random_ips();

public:
	S_INT_64	account_id_;
	boost::unordered_map<std::string, int> svrinfos;
	int			error_code_;

private:
	std::vector<std::string>	random_ip_;
	int cur_svr_;
};

#endif //__PHPRESULT_H__