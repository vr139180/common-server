#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/redis/DTaskRedis.h>

#include "sEurekaApp.h"

USED_REDISKEY_GLOBAL_NS

void ServiceRegisterCtrl::init_timer()
{
	//第一次初始化缓存
	this->redis_update_mth_serviemeta();

	//每3秒钟更新一次状态到redis
	svrApp.add_apptimer(TIMER_SERVICE_UPDATSYNC_STEP, boost::BOOST_BIND(&ServiceRegisterCtrl::service_sync_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	svrApp.add_apptimer(1000 * 6, boost::BOOST_BIND(&ServiceRegisterCtrl::service_maintnce_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void ServiceRegisterCtrl::service_sync_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	this->redis_update_mth_serviemeta();
}

void ServiceRegisterCtrl::service_maintnce_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	RedisClient* redis = svrApp.get_redisclient();

	//5 m
	static const S_INT_32 maintnce_step = 6;
	DTaskRedis dtask(GLOBAL_DTASK_SVRMAINTNCE_ALL, svrApp.get_eurekactrl()->get_myself().iid, maintnce_step);
	std::string taskkey = redis->build_rediskey(GLOBAL_DTASK_PREFIX, GLOBAL_DTASK_SVRMAINTNCE_ALL);

	bool bexit = false;
	//是否有任务在执行中
	if (redis->get<DTaskRedis>(taskkey.c_str(), dtask) == true)
		bexit = true;

	//触发时是否有其他任务创建
	if (redis->add<DTaskRedis>(taskkey.c_str(), dtask, maintnce_step * 1000, UpdateType::NOT_EXIST) == false)
		bexit = true;

	if (bexit)
	{
		finish = true;

		//结束当前定时器，调整下一个任务时间
		S_INT_32 timer = dtask.next_task_timer(3);
		if (timer == 0)
			return;

		svrApp.add_apptimer(timer, boost::BOOST_BIND(&ServiceRegisterCtrl::service_maintnce_timer, this,
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

		return;
	}

	this->maintnce_service(tnow);
}
