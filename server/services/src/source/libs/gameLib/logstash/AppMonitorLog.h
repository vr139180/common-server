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

#ifndef __APPMONITORLOG_H__
#define __APPMONITORLOG_H__

#include <gameLib/logstash/LogRecordBase.h>
#include <gameLib/protobuf/cpp/eureka_internal.pb.h>

class AppMonitorLog : public LogRecordBase
{
public:
	AppMonitorLog();

protected:
	virtual bool to_json(std::string& jstr);
	virtual std::string get_log_cat() { return out_monitor; }

protected:
	S_INT_64				service_iid_;
	PRO::ERK_SERVICETYPE	type_;
};

#endif //__APPMONITORLOG_H__
