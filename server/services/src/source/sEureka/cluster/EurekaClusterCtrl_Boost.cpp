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
	//��ʼ����Ϣ
	this->is_boosted_ = false;
	this->last_eureka_iid_ = 0;
	this->myself_.ismaster = false;
	this->myself_.iid = 0;
	this->eureka_master_iid_ = 0;

	//��������ע���Լ���ϵͳ
	myself_.ip = ConfigHelper::instance().get_ip();
	myself_.port = ConfigHelper::instance().get_port();
	myself_.token = (S_INT_64)OSSystem::mOS->GetTimestamp();

	//��ȡmaster�ڵ���Ϣ
	RedisClient* redis = svrApp.get_redisclient();
	S_INT_64 masterid = 0;
	if (!redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid))
	{
		//û��master�ڵ�,�����Լ�Ϊmaster
		myself_.ismaster = true;
		myself_.iid = this->make_next_eurekaiid();
		this->eureka_master_iid_ = myself_.iid;

		if (!update_redis_masterinfo(redis))
		{
			svrApp.quit_app();
			return false;
		}

		//���Ϊ�����ɹ�
		this->mark_boosted();

		//���浽�б���
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

	//���浽�б���
	try_regist_to_master(masterNode);
	
	return true;
}

void EurekaClusterCtrl::try_regist_to_master(EurekaNodeInfo info)
{
	//ע�ᵽeureka�ڵ��б���
	if (!is_legality_eureka(info.iid, info.token))
	{
		eureka_nodes_[info.iid] = info;

		std::list<EurekaNodeInfo> nodes;
		nodes.push_back(info);
		std::list<S_INT_64> deliids;
		eureka_links_to_.sync_eureka_services(nodes, deliids);
	}

	//master�ڵ���
	this->eureka_master_iid_ = info.iid;

	//ǿ�Ʒ�������
	eureka_links_to_.connect_to();
}
