#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/LogExt.h>

#include "sEurekaApp.h"
#include "debugdefine.h"

USED_REDISKEY_GLOBAL_NS

void EurekaClusterCtrl::update_redis_eurekas()
{
	for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = redis_nodes.begin(); iter != redis_nodes.end(); ++iter) {
		delete iter->second;
	}
	redis_nodes.clear();

	RedisClient* redis = svrApp.get_redisclient();

	boost::unordered_map<std::string, EurekaNodeInfo*> nods;
	redis->get_hashallobj<EurekaNodeInfo>(EUREKA_CLUSTER_ALLS, nods);

	bool missself = true;
	for (boost::unordered_map<std::string, EurekaNodeInfo*>::iterator iter = nods.begin(); iter != nods.end(); ++iter)
	{
		S_INT_64 sid = JSONUtil::str_2_int64(iter->first.c_str());
		EurekaNodeInfo* rinfo = iter->second;
		redis_nodes[sid] = rinfo;

		if (rinfo->iid == myself_.iid)
			missself = false;
	}

	if (missself) {
		//程序退出
		//WARNING
		logWarn(out_runtime, "!!!!!!!!!maintnce eureka node, self%lld(eureka) be removed from redis. will quit service....", myself_.iid);
		svrApp.quit_app();
		return;
	}

#ifdef EUREKA_DEBUGINFO_ENABLE
	logDebug(out_runtime, "updata redis eureka nodes size:%d", redis_nodes.size());
#endif

}

void EurekaClusterCtrl::maintnce_eurekanode_allcheck(S_TIMESTAMP tnow)
{
	RedisClient* redis = svrApp.get_redisclient();

	std::vector<std::string> invalidKeys;
	S_INT_64 checktime = ((S_INT_64)tnow - TIMER_EUREKA_QUEUE_STEP);
	if (redis->getbyscore_zset(EUREKA_CLUSTER_QUEUE, checktime, invalidKeys) == false)
		return;

	//从redis清楚 失效的eureka节点
	for (std::vector<std::string>::iterator iter = invalidKeys.begin(); iter != invalidKeys.end(); ++iter)
	{
		std::string skey = (*iter);

		redis->del_zsetmember(EUREKA_CLUSTER_QUEUE, skey.c_str());
		std::string bkey = redis->build_rediskey(EUREKA_CLUSTER_SVRBIND, JSONUtil::str_2_int64(skey.c_str()));
		redis->del(bkey.c_str());
		redis->del_hashmember(EUREKA_CLUSTER_ALLS, skey.c_str());

#ifdef EUREKA_DEBUGINFO_ENABLE
		logWarn(out_runtime, "sEureka[%ld] maintnce eurekanode check, find one Eureka node[%s] invalid. so remove all informations from redis.",
			myself_.iid, skey.c_str());
#endif
	}
}

void EurekaClusterCtrl::maintnce_sync_alleureka()
{
	this->update_redis_eurekas();

	RedisClient* redis = svrApp.get_redisclient();

	//根据现有的nodes来过滤可用连接
	//和其他eureka节点的线程安全,统一使用主线程的线程锁
	ThreadLockWrapper guard(svrApp.get_threadlock());

	std::list<IEurekaNodeLink*> removed;
	for (EUREKALINKNODE_MAP::iterator iter = link_nodes_.begin(); iter != link_nodes_.end(); ++iter)
	{
		S_INT_64 sid = iter->first;
		boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator fiter = redis_nodes.find(sid);
		if (fiter != redis_nodes.end())
			continue;
		removed.push_back(iter->second);
	}

	//移除无效的eureka节点
	for (std::list<IEurekaNodeLink*>::iterator iter = removed.begin(); iter != removed.end(); ++iter)
	{
		IEurekaNodeLink* pnode = (*iter);
#ifdef EUREKA_DEBUGINFO_ENABLE
		logInfo(out_runtime, "maintnce check find eureka service%lld=%lld need to be removed...", pnode->get_iid(), myself_.iid);
#endif

		//强制关闭连接
		pnode->force_linkclose();
	}
}

void EurekaClusterCtrl::warning_eureka_cantconnect(S_INT_64 toiid)
{

}

void EurekaClusterCtrl::warning_eurekabind_failed(S_INT_64 fromiid, S_INT_64 toiid)
{

}

void EurekaClusterCtrl::warning_eureka_disconnect(S_INT_64 fromiid, S_INT_64 toiid)
{

}