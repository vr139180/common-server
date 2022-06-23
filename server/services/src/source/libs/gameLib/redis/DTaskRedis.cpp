#include "gameLib/redis/DTaskRedis.h"

#include <cmsLib/json/JSONUtil.h>
#include <cmsLib/util/Random.h>
#include <cmsLib/base/OSSystem.h>

DTaskRedis::DTaskRedis():service_iid(0)
, start_time(0)
, task_step(0)
{
}

DTaskRedis::DTaskRedis(const char* name, S_INT_64 serviceiid, S_INT_32 step):start_time(0)
, service_iid(serviceiid)
, task_name(name)
, task_step(step)
{
	this->start_time = (S_INT_64)OSSystem::mOS->GetTimestamp();
}

bool DTaskRedis::to_json(std::string& val)
{
	boost::json::object json = {
		{"iid", service_iid},
		{"start", start_time},
		{"name", task_name},
		{"step", task_step}
	};

	val = boost::json::serialize(json);

	return true;
}

bool DTaskRedis::from_json(boost::json::value& root)
{
	boost::json::object obj = root.as_object();

	this->service_iid = JSONUtil::get_int64(obj, "iid", 0);
	this->start_time = JSONUtil::get_int64(obj, "start", 0);
	this->task_name = JSONUtil::get_string(obj, "name", "");
	this->task_step = JSONUtil::get_value<int>(obj, "step", 0);

	return true;
}

S_INT_32 DTaskRedis::next_task_timer(S_INT_32 randscope)
{
	if (this->task_step <= 0)
		return 0;

	S_TIMESTAMP t = OSSystem::mOS->GetTimestamp() - this->start_time - task_step*1000;
	S_INT_32 nt = 0;
	//超出下次执行时间
	if (t >= 0)
		nt = 1;	//==0 maybee return 0 at the endof this function
	else
		nt = (S_INT_32)(t * -1);

	if( randscope > 0)
		nt += CMS_RandMax(randscope * 10)*100;

	return nt;
}