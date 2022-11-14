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
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

EurekaLostMaintance* EurekaClusterCtrl::get_eurekanode_lost(S_INT_64 nodiid)
{
	boost::unordered_map<S_INT_64, EurekaLostMaintance*>::iterator fiter = lost_maintance_.find(nodiid);
	if (fiter == lost_maintance_.end())
		return 0;
	return fiter->second;
}

void EurekaClusterCtrl::fire_common_eurekanode_lost(S_INT_64 nodiid)
{
	//notify all eureka lost
	Erk_EurekaLost_ntf *ntf = new Erk_EurekaLost_ntf();
	std::unique_ptr<Erk_EurekaLost_ntf> ptr(ntf);
	ntf->set_from_eureka(myself_.iid);
	ntf->set_lost_eureka(nodiid);

	broadcast_to_eurekas(ntf);

	//已经在处理中
	if (get_eurekanode_lost(nodiid) != 0)
		return;

	//自己是master,或者对方是master
	if (!is_master() && eureka_master_iid_ != nodiid)
		return;

	//针对master选举的处理
	EurekaLostMaintance *plost = new EurekaLostMaintance(nodiid);
	lost_maintance_[plost->get_nodeiid()] = plost;
}

void EurekaClusterCtrl::tellme_eurekanode_lost(S_INT_64 fromiid, S_INT_64 lostnode)
{
	EurekaLostMaintance* plost = get_eurekanode_lost(lostnode);
	if (plost == 0)
		return;

	plost->somebody_tellme_lost(fromiid);

	//immediately check
	do_eurekanode_lost(lostnode);
}

void EurekaClusterCtrl::someone_eurekanode_authed(S_INT_64 nodiid)
{
	EurekaLostMaintance* plost = get_eurekanode_lost(nodiid);
	if (plost == 0)
		return;

	plost->rebind();

	do_eurekanode_lost(nodiid);
}

S_INT_32 EurekaClusterCtrl::check_eurekanode_lost_well(S_INT_64 lostnode)
{
	EurekaLostMaintance* plost = get_eurekanode_lost(lostnode);
	if (plost == 0)
		return 1; //lost

	if (plost->is_rebinded())
		return 2; //well

	if (plost->is_timeout())
		return 1; //lost

	if (plost->tellme_somebody_num() > 0)
		return 2; //lost

	if (plost->is_max_retry())
		return 1; //lost

	{
		S_INT_32 ens = this->eureka_node_size();
		//减去自己
		--ens;
		//减去丢失的个数
		ens -= lost_maintance_.size();

		//没有可以确认的
		if (ens <= 0)
			return 1; //lost
	}

	return 0; //waiting
}

void EurekaClusterCtrl::do_eurekanode_lost(S_INT_64 lostnode)
{
	S_INT_32 check = check_eurekanode_lost_well(lostnode);
	if (check == 0)
		return;

	//处理结束
	boost::unordered_map<S_INT_64, EurekaLostMaintance*>::iterator fiter = lost_maintance_.find(lostnode);
	if (fiter != lost_maintance_.end())
	{
		delete fiter->second;
		lost_maintance_.erase(fiter);
	}

	//恢复正常
	if (check == 2)
		return;

	//自己是master
	if (is_master())
	{
		//没有master数据或者master不再是自己了
		RedisClient* redis = svrApp.get_redisclient();
		S_INT_64 masterid = 0;
		if (!redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid) || masterid != myself_.iid)
		{
			//失效了
			svrApp.quit_app();
			return;
		}

		//对方失效
		master_invalid_one_slaver(lostnode);
	}
	else
	{
		//master节点还在,自己出问题了
		RedisClient* redis = svrApp.get_redisclient();
		S_INT_64 masterid = 0;
		if (redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid))
		{
			//失效了
			svrApp.quit_app();
			return;
		}

		//移除节点
		slaver_eurekanode_invalid(lostnode);

		//重选master
		slaver_vote_master();
	}
}

void EurekaClusterCtrl::slaver_vote_master()
{
	if (vote_key_.is_validate())
		return;

	vote_master_retry_ = 0;
	slaver_try_vote_master();
}

void EurekaClusterCtrl::vote_master_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	finish = true;
	vote_key_ = TimerKey::None;

	slaver_try_vote_master();
}

void EurekaClusterCtrl::slaver_try_vote_master()
{
	++vote_master_retry_;

	//一直选不出master退出
	if (vote_master_retry_ > 10)
	{
		svrApp.quit_app();
		return;
	}

	//选举
	

	vote_key_ = svrApp.add_apptimer(1000 * 1, boost::BOOST_BIND(&EurekaClusterCtrl::vote_master_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}