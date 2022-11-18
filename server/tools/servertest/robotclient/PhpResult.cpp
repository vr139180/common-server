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

#include "PhpResult.h"

#include <cmsLib/util/ShareUtil.h>
#include <cmsLib/util/Random.h>

PhpResult* PhpResult::build_from_str(const char* str)
{
	try {
		boost::json::parse_options opt;
		opt.allow_comments = true;
		opt.allow_trailing_commas = true;

		boost::json::error_code ec;
		boost::json::value root = boost::json::parse(str, ec, {}, opt);
		if (ec.failed())
		{
			return 0;
		}

		PhpResult* ret = new PhpResult();
		std::unique_ptr<PhpResult> ptr(ret);

		boost::json::object& obj = root.as_object();
		int result = JSONUtil::get_value<int>(obj, "code", 1);
		ret->error_code_ = result;
		if (result == 0)
		{
			if (!obj.contains("data"))
				return 0;

			boost::json::object& data = obj.at("data").as_object();

			ret->account_id_ = JSONUtil::get_int64(data, "useriid", 0);

			if (!data.contains("gates"))
				return 0;

			std::string strips = data.at("gates").as_string().c_str();
			std::list<std::string> ips;
			ShareUtil::splitstr2str(strips.c_str(), ",", ips);

			for (std::list<std::string>::iterator iter = ips.begin(); iter != ips.end(); ++iter)
			{
				std::string strip = (*iter);
				std::list<std::string> ipports;
				ShareUtil::splitstr2str(strip.c_str(), ":", ipports);
				std::list<std::string>::iterator iter2 = ipports.begin();
				std::string ip = *(iter2);
				++iter2;
				std::string strport = *(iter2);
				int port = ShareUtil::atoi(strport.c_str());

				ret->svrinfos[ip] = port;
			}

			ret->random_ips();

			if (ret->svrinfos.size() == 0)
				return 0;
		}

		return ptr.release();
	}
	catch (...) {
		return 0;
	}
}

PhpResult::~PhpResult()
{
	svrinfos.clear();
	random_ip_.clear();
}

void PhpResult::random_ips()
{
	if (svrinfos.size() == 0)
		return;

	for (boost::unordered_map<std::string, int>::iterator iter = svrinfos.begin(); iter != svrinfos.end(); ++iter)
	{
		random_ip_.push_back(iter->first);
	}

	std::random_shuffle(random_ip_.begin(), random_ip_.end());
	cur_svr_ = 0;
}

void PhpResult::get_ipinfo(std::string& ip, int& port)
{
	if (random_ip_.size() == 0)
		return;

	if (cur_svr_ >= random_ip_.size())
		cur_svr_ = 0;

	ip = random_ip_.at(cur_svr_);
	port = svrinfos[ip];
	++cur_svr_;
}
