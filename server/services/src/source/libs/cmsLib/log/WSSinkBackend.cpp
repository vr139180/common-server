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

#include <cmsLib/log/WSSinkBackend.h>

#include <boost/log/expressions.hpp>
#include <cmsLib/util/ShareUtil.h>

WSSinkBackend::~WSSinkBackend()
{
}

void WSSinkBackend::consume(logging::record_view const& rec)
{
	const std::string& msg = *rec[logging::expressions::smessage];
	if (rec.attribute_values().count("Model") > 0)
	{
		logging::attribute_value model = rec.attribute_values()["Model"];
	}
}

void WSSinkBackend::flush()
{

}

void WSSinkBackend::add_model_url(std::string model, std::string url)
{
	ShareUtil::to_lower(model);
	models_[model] = url;
}
