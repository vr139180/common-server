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

#include <gameLib/logstash/GameDataLog.h>

#include <cmsLib/json/JSONUtil.h>

GameDataLog::GameDataLog() :LogRecordBase()
{
}

bool GameDataLog::to_json(std::string& jstr)
{
	boost::json::object json = {
		{"createtime", now_}
	};

	jstr = boost::json::serialize(json);

	return true;
}
