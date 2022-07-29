#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>

#include "sEurekaApp.h"

USED_REDISKEY_GLOBAL_NS

bool EurekaClusterCtrl::init_ctrl()
{
	RedisClient* redis = svrApp.get_redisclient();

	//必须首先注册自己到系统
	myself_.iid = redis->gen_uuid_from_redis(GLOBAL_IDGEN_HKEY, GLOBAL_IDGEN_SERVICE_F);
	myself_.ip = ConfigHelper::instance().get_ip();
	myself_.port = ConfigHelper::instance().get_port();
	myself_.token = (S_INT_64)OSSystem::mOS->GetTimestamp();

	redis->set_hashobject<EurekaNodeInfo>(EUREKA_CLUSTER_ALLS, std::to_string(myself_.iid).c_str(), myself_);
	
	//注册到queue
	//score 取负数,方便zset计算
	this->lastupdate_ = myself_.token;
	S_INT_64 checktime = this->lastupdate_;
	redis->add_zset(EUREKA_CLUSTER_QUEUE, std::to_string(myself_.iid).c_str(), checktime);
	
	//从系统获取存在的cluster节点
	//***eureka节点永远只在启动的时候设置一次连接请求
	update_redis_eurekas();

	logInfo(out_runtime, "<<<<<<<<<<<< eureka node:%lld online >>>>>>>>>>>>", myself_.iid);

	//系统初始化时，可能存在上次遗留的同机器的服务，需要判断ip+port是否是本机
	for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = redis_nodes.begin(); iter != redis_nodes.end(); ++iter)
	{
		S_INT_64 sid = iter->first;
		EurekaNodeInfo* rinfo = iter->second;

		//过滤自己
		if (sid == myself_.iid)
			continue;
		//如果连接的是本机，过滤自己
		if (rinfo->ip == myself_.ip && rinfo->port == myself_.port)
		{
			continue;
		}

		{
			//和其他eureka节点的线程安全,统一使用主线程的线程锁
			ThreadLockWrapper guard( svrApp.get_threadlock());

			rinfo = rinfo->clone();
			EurekaLinkTo* lto = new EurekaLinkTo(rinfo);
			links_to_.push_back(lto);
			auth_links_to_.insert(lto);

			lto->connect();
		}
	}

	return true;
}

void EurekaClusterCtrl::unint_ctrl()
{
	RedisClient* redis = svrApp.get_redisclient();

	redis->del_hashmember(EUREKA_CLUSTER_ALLS, std::to_string(myself_.iid).c_str());
	redis->del_zsetmember(EUREKA_CLUSTER_QUEUE, std::to_string(myself_.iid).c_str());

	std::string key = redis->build_rediskey(EUREKA_CLUSTER_SVRBIND, myself_.iid);
	redis->del(key.c_str());
}

