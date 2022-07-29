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

	//��������ע���Լ���ϵͳ
	myself_.iid = redis->gen_uuid_from_redis(GLOBAL_IDGEN_HKEY, GLOBAL_IDGEN_SERVICE_F);
	myself_.ip = ConfigHelper::instance().get_ip();
	myself_.port = ConfigHelper::instance().get_port();
	myself_.token = (S_INT_64)OSSystem::mOS->GetTimestamp();

	redis->set_hashobject<EurekaNodeInfo>(EUREKA_CLUSTER_ALLS, std::to_string(myself_.iid).c_str(), myself_);
	
	//ע�ᵽqueue
	//score ȡ����,����zset����
	this->lastupdate_ = myself_.token;
	S_INT_64 checktime = this->lastupdate_;
	redis->add_zset(EUREKA_CLUSTER_QUEUE, std::to_string(myself_.iid).c_str(), checktime);
	
	//��ϵͳ��ȡ���ڵ�cluster�ڵ�
	//***eureka�ڵ���Զֻ��������ʱ������һ����������
	update_redis_eurekas();

	logInfo(out_runtime, "<<<<<<<<<<<< eureka node:%lld online >>>>>>>>>>>>", myself_.iid);

	//ϵͳ��ʼ��ʱ�����ܴ����ϴ�������ͬ�����ķ�����Ҫ�ж�ip+port�Ƿ��Ǳ���
	for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = redis_nodes.begin(); iter != redis_nodes.end(); ++iter)
	{
		S_INT_64 sid = iter->first;
		EurekaNodeInfo* rinfo = iter->second;

		//�����Լ�
		if (sid == myself_.iid)
			continue;
		//������ӵ��Ǳ����������Լ�
		if (rinfo->ip == myself_.ip && rinfo->port == myself_.port)
		{
			continue;
		}

		{
			//������eureka�ڵ���̰߳�ȫ,ͳһʹ�����̵߳��߳���
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

