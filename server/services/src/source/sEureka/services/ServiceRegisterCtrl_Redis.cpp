#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/redis/global_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/global_const.h>
#include <gameLib/LogExt.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

void ServiceRegisterCtrl::redis_update_mth_serviemeta()
{
	RedisClient* redis = svrApp.get_redisclient();

	for (int ind = (int)(NETSERVICE_TYPE::ERK_SERVICE_NONE) + 1; ind < NETSERVICE_TYPE::ERK_SERVICE_MAX; ++ind)
	{
		NETSERVICE_TYPE itype = (NETSERVICE_TYPE)ind;

		std::string skey = redis->build_rediskey(SERVICE_CATEGORY, NetServiceType::to_string(itype).c_str());
		boost::unordered_map<std::string, ServiceNodeInfo*> infos;
		if (redis->get_hashallobj<ServiceNodeInfo>(skey.c_str(), infos) == false)
		{
			continue;
		}

		std::set<S_INT_64> delsvrs;
		this->service_mth_meta_merge_oftype(itype, infos, delsvrs);

		if (delsvrs.size() > 0)
		{
			ThreadLockWrapper guard(svrApp.get_threadlock());
			for (std::set<S_INT_64>::iterator iter = delsvrs.begin(); iter != delsvrs.end(); ++iter)
			{
				S_INT_64 iid = (*iter);
#ifdef EUREKA_DEBUGINFO_ENABLE
				logError(out_runtime, "--------------force close lost service------");
#endif
				//force close
				ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(iid);
				if (plink != 0)
					plink->force_close();
			}
		}
	}
}

void ServiceRegisterCtrl::redis_serviceregist_do2(S_INT_64 sid)
{
	RedisClient* redis = svrApp.get_redisclient();

	std::string svrid = std::to_string(sid);
	std::string skey = redis->build_rediskey(SERVICE_DETAIL_INFO, sid);
	if (!redis->persist(skey.c_str()))
	{
		logError(out_runtime, "!!!!!!!service regist step 2, data of the service on the redis is timeout!!!!!!");
		return;
	}

	ServiceNodeInfo pnode;
	if (redis->get_hashobject(skey.c_str(), SERVICE_DETAIL_SVRINFO, pnode) == false)
		return;
	if (pnode.iid != sid)
		return;

	//添加服务信息到 分类中
	skey = redis->build_rediskey(SERVICE_CATEGORY, NetServiceType::to_string(pnode.type).c_str());
	redis->set_hashobject<ServiceNodeInfo>(skey.c_str(), svrid.c_str(), pnode);

	//写更新时间到queue
	{
		skey = redis->build_rediskey(SERVICE_MAINTNCE, NetServiceType::to_string(pnode.type).c_str());
		S_INT_64 lasttime = OSSystem::mOS->GetTimestamp();
		redis->add_zset(skey.c_str(), std::to_string(pnode.iid).c_str(), lasttime, UpdateType::ALWAYS);
	}

}

void ServiceRegisterCtrl::redis_serviceregist_do1(ServiceNodeInfo* pnode)
{
	RedisClient* redis = svrApp.get_redisclient();

	//创建一个service的详细数据项
	std::string skey = redis->build_rediskey(SERVICE_DETAIL_INFO, pnode->iid);
	//base
	redis->set_hashmember(skey.c_str(),
		{
			std::make_pair(SERVICE_DETAIL_TOKEN, std::to_string(pnode->token).c_str()),
			std::make_pair(SERVICE_DETAIL_STATUS, "0"),
			std::make_pair(SERVICE_DETAIL_SVRTYPE, NetServiceType::to_string(pnode->type).c_str()),
		});
	redis->set_hashobject(skey.c_str(), SERVICE_DETAIL_SVRINFO, *pnode);

	if (pnode->type == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		//gate sequence iid
		redis->set_hashmember(skey.c_str(), SERVICE_GATEEXT_SLOTSEQ, "0");
	}

	//设置3分钟超时时间
	redis->pexpire(skey.c_str(), 3 * 60 * 1000);
}
