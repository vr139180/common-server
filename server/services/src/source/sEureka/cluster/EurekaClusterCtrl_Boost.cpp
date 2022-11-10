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

USED_REDISKEY_GLOBAL_NS

bool EurekaClusterCtrl::boot_ctrl()
{
	//初始化信息
	this->is_boosted_ = false;
	this->last_eureka_iid_ = 0;
	this->myself_.ismaster = false;
	this->myself_.iid = 0;
	this->eureka_master_iid_ = 0;

	//必须首先注册自己到系统
	myself_.ip = ConfigHelper::instance().get_ip();
	myself_.port = ConfigHelper::instance().get_port();
	myself_.token = (S_INT_64)OSSystem::mOS->GetTimestamp();

	//获取master节点信息
	RedisClient* redis = svrApp.get_redisclient();
	S_INT_64 masterid = 0;
	if (!redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid))
	{
		//没有master节点,设置自己为master
		myself_.ismaster = true;
		myself_.iid = this->make_next_eurekaiid();
		this->eureka_master_iid_ = myself_.iid;

		if (!update_redis_masterinfo(redis))
		{
			svrApp.quit_app();
			return false;
		}

		//标记为启动成功
		this->mark_boosted();

		//保存到列表中
		eureka_nodes_[myself_.iid] = myself_;

		logInfo(out_runtime, "<<<<<<<<<<<< eureka node:%lld online >>>>>>>>>>>>", myself_.iid);

		return true;
	}

	EurekaNodeInfo masterNode;
	if (!redis->get_hashobject(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_SVR, masterNode))
	{
		svrApp.quit_app();
		return false;
	}

	//保存到列表中
	try_regist_to_master(masterNode);
	
	return true;
}

void EurekaClusterCtrl::try_regist_to_master(EurekaNodeInfo info)
{
	//注册到eureka节点列表中
	if (!is_legality_eureka(info.iid, info.token))
	{
		eureka_nodes_[info.iid] = info;

		std::list<EurekaNodeInfo> nodes;
		nodes.push_back(info);
		std::list<S_INT_64> deliids;
		eureka_links_to_.sync_eureka_services(nodes, deliids);
	}

	//master节点编号
	this->eureka_master_iid_ = info.iid;

	//强制发起连接
	eureka_links_to_.connect_to();
}
