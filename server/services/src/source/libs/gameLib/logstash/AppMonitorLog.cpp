#include <gameLib/logstash/AppMonitorLog.h>

#include <cmsLib/json/JSONUtil.h>

AppMonitorLog::AppMonitorLog() :LogRecordBase()
{
}

bool AppMonitorLog::to_json(std::string& jstr)
{
	boost::json::object json = {
		{"sid", service_iid_},
		{"stype", (int)type_},
		{"createtime", now_}
	};

	jstr = boost::json::serialize(json);

	return true;
}
