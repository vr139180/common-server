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
