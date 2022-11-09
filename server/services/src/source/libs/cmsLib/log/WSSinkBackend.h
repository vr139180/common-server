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

#ifndef __WSSINKBACKEND_H__
#define __WSSINKBACKEND_H__

#include <boost/unordered_map.hpp>
#include <boost/log/sinks/frontend_requirements.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>

#include <cmsLib/httpcurl/HttpClient.h>

namespace logging = boost::log;
namespace sinks = logging::sinks;

/*
[Sinks.CmsWebService]
Destination=WebService

Filter="%Model% matches gd or %Model% matches mt"

Models=gd;mt
Model_gd_inf=
Model_mt_inf=

Asynchronous=true
*/
class WSSinkBackend : public sinks::basic_sink_backend<
	sinks::combine_requirements<sinks::synchronized_feeding,sinks::flushing>::type>
{
	typedef boost::unordered_map<std::string, std::string> MODEL_URLS;
public:
	virtual ~WSSinkBackend();

	void consume(logging::record_view const& rec);

	void flush();

	void add_model_url(std::string model, std::string url);

private:
	MODEL_URLS	models_;

	HttpClient	ws_client_;
};

#endif //__WSSINKBACKEND_H__
