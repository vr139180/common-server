#include <gameLib/logstash/LogRecordBase.h>

#include <cmsLib/base/OSSystem.h>

LogRecordBase::LogRecordBase()
{
	now_ = OSSystem::mOS->GetTimestamp();
}

void LogRecordBase::save()
{
	std::string jstr;
	bool bsucc = to_json(jstr);
	if (!bsucc)
		return;

	std::string cat = get_log_cat();
	logWarn(cat.c_str(), jstr.c_str());
}
