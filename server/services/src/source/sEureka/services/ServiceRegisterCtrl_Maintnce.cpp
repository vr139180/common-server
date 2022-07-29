#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/redis/global_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/global_const.h>

#include "sEurekaApp.h"
#include "rediscript/homegate_script.h"

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

			if (ii == (int)NETSERVICE_TYPE::ERK_SERVICE_GATE || ii == (int)NETSERVICE_TYPE::ERK_SERVICE_HOME)
			{
				redis_gatebindhome_maintnce(ShareUtil::atoi64(skey.c_str()), (NETSERVICE_TYPE)ii);
			}

			logWarn(out_runtime, "sEureka[%ld] maintnce service node check, find one Service node[%s] invalid. so remove all informations from redis.",
				svrApp.get_eurekactrl()->get_myself().iid, skey.c_str());
		}
	}
}

void ServiceRegisterCtrl::redis_gatebindhome_maintnce(S_INT_64 sid, NETSERVICE_TYPE type)
{
	RedisClient* redis = svrApp.get_redisclient();

	std::initializer_list<StringView> keys =
	{
		HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,GLOBAL_USERSLOT_FREE,
		HOMEGATE_MIX,HOMEGATE_WATERSLOT,HOMEGATE_FLEXSLOT
	};

	std::string ssid = std::to_string(sid);
	std::string stype;
	if (type == NETSERVICE_TYPE::ERK_SERVICE_GATE)
		stype = "1";
	else
		stype = "2";

	std::string sgnum = std::to_string(GATEHOME_GROUP_NUM);
	std::string stotle = std::to_string(MIN_GATEHOME_GROUP_NUM);
	std::initializer_list<StringView> vals =
	{
		ssid.c_str(),stype.c_str(), "0",
		sgnum.c_str(),stotle.c_str(),"0"
	};
	std::vector<std::string> rets;
	if (redis->evalStrs(HomeGateConst::gatebindhome_maintnce.c_str(), keys, vals, rets))
	{
		//logDebug(out_runtime, "gatebindhome maintnce, redis eval result:%s", rets[0].c_str());
		int ret = ShareUtil::atoi(rets[0].c_str());
		if (ret == 0 && rets.size() == 3)
		{
			S_INT_64 homeid = ShareUtil::atoi64(rets[1].c_str());
			S_INT_64 gateid = ShareUtil::atoi64(rets[2].c_str());
			std::string sau = redis->build_rediskey(HOMEGATE_USERSLOT_AUTH, homeid, gateid);
			redis->del(sau.c_str());
		}
	}
}

void ServiceRegisterCtrl::maintnce_gatebindhome_authtimeout(u64 tnow)
{
	//logDebug(out_runtime, "gate bind home auth timeout check");
	RedisClient* redis = svrApp.get_redisclient();

	std::string hqueue = redis->build_rediskey(SERVICE_MAINTNCE,
		NetServiceType::to_string(NETSERVICE_TYPE::ERK_SERVICE_HOME).c_str());
	std::string gqueue = redis->build_rediskey(SERVICE_MAINTNCE,
		NetServiceType::to_string(NETSERVICE_TYPE::ERK_SERVICE_GATE).c_str());

	std::initializer_list<StringView> keys =
	{
		HOMEGATE_GATE_WAIT,HOMEGATE_HOME_WAIT,HOMEGATE_AUTHING,
		hqueue.c_str(), gqueue.c_str()
	};

	std::vector<std::string> invalidKeys;
	//auth认证不超过5s 
	S_INT_64 checktime = ((S_INT_64)tnow - 5000);
	if (redis->getbyscore_zset(HOMEGATE_AUTHING, checktime, invalidKeys) == false)
		return;

	for (std::vector<std::string>::iterator iter = invalidKeys.begin(); iter != invalidKeys.end(); ++iter)
	{
		std::string skey = (*iter);
		std::vector<S_INT_64> iids;
		ShareUtil::splitstr2int64( skey.c_str(), "#", iids);
		if (iids.size() != 2)
		{
			logError(out_runtime, "XXXXXXXXXX  HOMEGATE_AUTHING queue have an error data:%s", skey.c_str());
			continue;
		}

		S_INT_64 homeid = iids[0];
		S_INT_64 gateid = iids[1];

		std::string sgateid = std::to_string(gateid);
		std::string shomeid = std::to_string(homeid);
		std::initializer_list<StringView> vals =
		{
			sgateid.c_str(),shomeid.c_str(),"0",
			"0","0"
		};
		std::vector<std::string> rets;
		if (redis->evalStrs(HomeGateConst::gatebindhome_authtimeout.c_str(), keys, vals, rets))
		{
			//logDebug(out_runtime, "gate bind home auth:%s timeout check result:%s", skey.c_str(), rets[0].c_str());
		}
		else {
			logError(out_runtime, "gate bind home auth timeout check failed");
		}
	}
}