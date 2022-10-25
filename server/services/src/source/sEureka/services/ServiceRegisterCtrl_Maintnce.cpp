#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/redis/global_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/global_const.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

void ServiceRegisterCtrl::maintnce_service(u64 tnow)
{
	RedisClient* redis = svrApp.get_redisclient();

	for (int ii = (int)NETSERVICE_TYPE::ERK_SERVICE_NONE + 1; ii < (int)NETSERVICE_TYPE::ERK_SERVICE_MAX; ++ii)
	{
		std::string svrtype = NetServiceType::to_string((NETSERVICE_TYPE)ii);

		std::string zkey = redis->build_rediskey(SERVICE_MAINTNCE, svrtype.c_str());

		std::vector<std::string> invalidKeys;
		S_INT_64 checktime = ((S_INT_64)tnow - TIMER_SERVICE_QUEUE_STEP);
		if (redis->getbyscore_zset(zkey.c_str(), checktime, invalidKeys) == false)
			continue;

		//从redis清楚 失效的eureka节点
		for (std::vector<std::string>::iterator iter = invalidKeys.begin(); iter != invalidKeys.end(); ++iter)
		{
			std::string skey = (*iter);

			redis->del_zsetmember(zkey.c_str(), skey.c_str());

			//del from _svrs:%type%
			std::string rkey = redis->build_rediskey(SERVICE_CATEGORY, svrtype.c_str());
			redis->del_hashmember(rkey.c_str(), skey.c_str());

			//del _service:id
			rkey = redis->build_rediskey(SERVICE_DETAIL_INFO, atoll(skey.c_str()));
			redis->del(rkey.c_str());

			logWarn(out_runtime, "sEureka[%ld] maintnce service node check, find one Service node[%s] invalid. so remove all informations from redis.",
				svrApp.get_eurekactrl()->get_myself().iid, skey.c_str());
		}
	}
}
