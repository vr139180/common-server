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
	//已经在处理中
	if (get_eurekanode_lost(nodiid) != 0)
		return;

	//自己是master,或者对方是master. all not
	if (!(is_master() || eureka_master_iid_ == nodiid))
		return;

	//针对master选举的处理
	EurekaLostMaintance *plost = new EurekaLostMaintance(nodiid, eureka_master_iid_);
	lost_maintance_[plost->get_nodeiid()] = plost;
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
		return 1; //ignore

	if (plost->is_rebinded())
		return 2; //well

	if (plost->is_timeout() || plost->is_max_retry())
		return 4; //lost

	//已经丢失了
	S_INT_64 masteriid = redis_have_masterinfo(0);
	if (masteriid == 0)
		return 3; //can try vote
	else if (!plost->is_same_master(masteriid))
		return 4;//lost,已经选出了master

	return 0; //waiting
}

void EurekaClusterCtrl::do_eurekanode_lost(S_INT_64 lostnode)
{
	S_INT_32 check = check_eurekanode_lost_well(lostnode);
	if (check == 1 || check == 0)
		return;

	if (check == 2 || check == 4)
	{
		//处理结束
		boost::unordered_map<S_INT_64, EurekaLostMaintance*>::iterator fiter = lost_maintance_.find(lostnode);
		if (fiter != lost_maintance_.end())
		{
			delete fiter->second;
			lost_maintance_.erase(fiter);
		}
	}

	//恢复正常
	if (check == 2)
		return;

	//自己是master
	if (is_master())
	{
		//check=3不可能
		//没有master数据或者master不再是自己了
		S_INT_64 masterid = redis_have_masterinfo(0);
		if (masterid == 0 || masterid != eureka_master_iid_)
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
		//移除节点
		slaver_eurekanode_invalid(lostnode);

		//重选master
		slaver_try_vote_master();
	}
}

void EurekaClusterCtrl::slaver_try_vote_master()
{
	//如果没有任何连接，直接退出
	//状态更清晰
	if (svrApp.get_servicectrl()->get_linkedservice_size() == 0 && svrApp.get_servicectrl()->get_service_node_size() > 0)
	{
		svrApp.quit_app();
		return;
	}
	//孤立的节点
	if (eureka_nodes_.size() == 0 && svrApp.get_servicectrl()->get_service_node_size() == 0)
	{
		svrApp.quit_app();
		return;
	}

	//获取最小的节点
	S_INT_64 min_iid = get_myself().iid;
	for (boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator iter = eureka_nodes_.begin(); iter != eureka_nodes_.end(); ++iter)
	{
		S_INT_64 iid = iter->first;
		if( min_iid > iid)
			min_iid = iid;
	}
	
	//只选取最小的
	if (min_iid != get_myself().iid)
	{
		//没有和其他slaver连接，退出
		S_INT_32 nums = eureka_links_from_.get_linked_size() + eureka_links_to_.get_authed_size();
		if( nums == 0)
			svrApp.quit_app();

		return;
	}

	make_me_masternode();
}