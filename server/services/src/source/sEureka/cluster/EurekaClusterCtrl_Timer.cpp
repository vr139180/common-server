// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

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
	svrApp.add_apptimer(1000 * 2, boost::BOOST_BIND(&EurekaClusterCtrl::eureka_auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void EurekaClusterCtrl::eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	if (is_master() && is_boosted())
	{
		//定时延期
		redis_active_masterinfo(0);
	}

	//断线维护
	if (lost_maintance_.size() > 0)
	{
		std::vector<S_INT_64> emiids;
		for (boost::unordered_map<S_INT_64, EurekaLostMaintance*>::iterator iter = lost_maintance_.begin(); iter != lost_maintance_.end(); ++iter)
		{
			EurekaLostMaintance* pl = iter->second;
			pl->try_reconnect();

			emiids.push_back(iter->first);
		}

		for (int ii = 0; ii < emiids.size(); ++ii)
		{
			S_INT_64 lostnode = emiids[ii];
			do_eurekanode_lost(lostnode);
		}
	}

	//其他重连情况
	eureka_links_to_.connect_to();
}
