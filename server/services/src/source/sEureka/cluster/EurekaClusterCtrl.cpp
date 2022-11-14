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

#include "sEurekaApp.h"
#include "debugdefine.h"

USED_REDISKEY_GLOBAL_NS

EurekaClusterCtrl::EurekaClusterCtrl():is_boosted_(false)
, eureka_master_iid_(0)
, last_eureka_iid_(0)
{
	vote_key_ = TimerKey::None;
	vote_master_retry_ = 0;
}

EurekaClusterCtrl::~EurekaClusterCtrl()
{
	release();
}

void EurekaClusterCtrl::init_ctrl()
{
	eureka_links_from_.init_holder();

	this->InitNetMessage();
}

void EurekaClusterCtrl::unint_ctrl()
{
	//master结束时删除redis注册信息
	if (is_master())
	{
		RedisClient* redis = svrApp.get_redisclient();

		S_INT_64 masterid = 0;
		if (redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid))
		{
			if (myself_.iid == masterid)
			{
				redis->del(EUREKA_MASTER_NODE);
			}
		}
	}

	for (boost::unordered_map<S_INT_64, EurekaLostMaintance*>::iterator iter = lost_maintance_.begin(); iter != lost_maintance_.end(); ++iter)
		delete iter->second;
	lost_maintance_.clear();

	eureka_links_from_.uninit_holder();
	eureka_links_to_.free_all();
}

void EurekaClusterCtrl::release()
{
	eureka_nodes_.clear();
}

bool EurekaClusterCtrl::check_node_is_master(S_INT_64 nodeid)
{
	if (nodeid <= 0 || eureka_master_iid_ <= 0)
		return false;

	return nodeid == eureka_master_iid_;
}

bool EurekaClusterCtrl::is_eurekalink_exist(S_INT_64 iid)
{
	if (iid == myself_.iid)
		return true;

	if (iid > myself_.iid)
	{
		return eureka_links_from_.get_eurekalink_byiid(iid) > 0;
	}
	else
	{
		return eureka_links_to_.get_eurekalink_byiid(iid) > 0;
	}
}

bool EurekaClusterCtrl::is_eureka_node(S_INT_64 iid, S_INT_64 token)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(iid);
	if (fiter == eureka_nodes_.end())
		return false;
	EurekaNodeInfo& ni = fiter->second;
	return ni.is_same_node(token);
}

EurekaNodeInfo* EurekaClusterCtrl::get_eureka_node(S_INT_64 iid)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(iid);
	if (fiter == eureka_nodes_.end())
		return 0;
	return &(fiter->second);
}

bool EurekaClusterCtrl::is_eureka_node(S_INT_64 iid)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(iid);
	if (fiter == eureka_nodes_.end())
		return false;
	return true;
}

void EurekaClusterCtrl::broadcast_to_eurekas(BasicProtocol* msg, IEurekaNodeLink* ignore)
{
	eureka_links_from_.broadcast(msg,(ignore==0?0: ignore->get_iid()));
	eureka_links_to_.broadcast(msg, (ignore == 0 ? 0 : ignore->get_iid()));
}

S_INT_64 EurekaClusterCtrl::make_next_eurekaiid()
{
	S_INT_64 iid = ++last_eureka_iid_;
	if (is_eureka_node(iid))
		iid = make_next_eurekaiid_fix();
	return iid;
}

void EurekaClusterCtrl::on_disconnected_with_linkto(EurekaLinkTo* plink)
{
	bool isready = plink->is_ready();
	S_INT_64 iid = plink->get_iid();

	eureka_links_to_.on_linkto_disconnected(plink);

	if (isready)
		fire_common_eurekanode_lost( iid);
}

void EurekaClusterCtrl::on_linkto_regist_result(EurekaLinkTo* plink)
{
	eureka_links_to_.on_linkto_regist_result(plink);
}

void EurekaClusterCtrl::on_disconnected_with_linkfrom(EurekaLinkFrom* plink)
{
	bool isready = plink->is_ready();
	S_INT_64 iid = plink->get_iid();

	//结合sEurekaApp的disconnect处理流程
	eureka_links_from_.return_freelink(plink);

	if (isready)
		fire_common_eurekanode_lost(iid);
}

bool EurekaClusterCtrl::force_reconnect(S_INT_64 iid)
{
	return eureka_links_to_.connect_to( iid);
}

S_INT_32 EurekaClusterCtrl::get_lived_eurekanode_size()
{
	return eureka_links_from_.get_linked_size() + eureka_links_to_.get_authed_size();
}