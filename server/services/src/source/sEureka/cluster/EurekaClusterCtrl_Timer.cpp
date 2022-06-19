#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/redis/DTaskRedis.h>

#include "sEurekaApp.h"

USED_REDISKEY_GLOBAL_NS

void EurekaClusterCtrl::init_timer()
{
	svrApp.add_apptimer(1000 * 5, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	svrApp.add_apptimer(1000 * 15, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_linkto_heart_beat, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	//每3秒钟更新一次状态到redis
	svrApp.add_apptimer(TIMER_EUREKA_UPDATELASTIME_STEP, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_update_heartbeat_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	//每10秒钟调用一次状态维护,分布式任务,cluster节点只有一个能执行
	//每个任务有效期5秒，5秒之后redis释放任务所有权
	svrApp.add_apptimer(1000 * 5, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_maintnce_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void EurekaClusterCtrl::eureka_linkto_heart_beat(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(svrApp.get_threadlock());

	for (std::list<EurekaLinkTo*>::iterator iter = links_to_.begin(); iter != links_to_.end(); ++iter)
	{
		EurekaLinkTo* pto = (*iter);
		if (!pto->is_authed())
			continue;

		pto->heart_beat();
	}
}

void EurekaClusterCtrl::eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	if (wait_links_to_.size() == 0)
		return;

	ThreadLockWrapper guard(svrApp.get_threadlock());

	for (std::set<EurekaLinkTo*>::iterator iter = wait_links_to_.begin(); iter != wait_links_to_.end(); ++iter)
	{
		EurekaLinkTo* plink = (*iter);
		plink->connect();

		auth_links_to_.insert(plink);
	}

	wait_links_to_.clear();
}

void EurekaClusterCtrl::eureka_update_heartbeat_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	RedisClient* redis = svrApp.get_redisclient();

	//更新时间戳, zset计算
	this->lastupdate_ = (S_INT_64)tnow;
	S_INT_64 zsettime = this->lastupdate_;
	//仅在key存在的情况下更新
	redis->add_zset(EUREKA_CLUSTER_QUEUE, std::to_string(myself_.iid).c_str(), zsettime, UpdateType::EXIST);

	this->maintnce_sync_alleureka();
}

void EurekaClusterCtrl::eureka_maintnce_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	RedisClient* redis = svrApp.get_redisclient();

	//5 m
	static const S_INT_32 maintnce_step = 5;
	DTaskRedis dtask(GLOBAL_DTASK_EUREKAMAINTNCE, myself_.iid, maintnce_step);
	std::string taskkey = redis->build_rediskey(GLOBAL_DTASK_PREFIX, GLOBAL_DTASK_EUREKAMAINTNCE);

	bool bexit = false;
	//是否有任务在执行中
	if (redis->get<DTaskRedis>(taskkey.c_str(), dtask) == true)
		bexit = true;

	//触发时是否有其他任务创建
	if (redis->add<DTaskRedis>(taskkey.c_str(), dtask, maintnce_step*1000, UpdateType::NOT_EXIST) == false)
		bexit = true;

	if (bexit)
	{
		finish = true;

		//结束当前定时器，调整下一个任务时间
		S_INT_32 timer = dtask.next_task_timer(3);
		if (timer == 0)
			return;

		svrApp.add_apptimer(timer, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_maintnce_timer, this,
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

		return;
	}

	this->maintnce_eurekanode_allcheck( tnow);
}
