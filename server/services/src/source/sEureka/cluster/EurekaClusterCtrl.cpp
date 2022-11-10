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

EurekaClusterCtrl::EurekaClusterCtrl():lastupdate_(0)
, is_boosted_(false)
, eureka_master_iid_(0)
, last_eureka_iid_(0)
{

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

bool EurekaClusterCtrl::is_eureka_exist(S_INT_64 iid)
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

bool EurekaClusterCtrl::is_legality_eureka(S_INT_64 iid, S_INT_64 token)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(iid);
	if (fiter == eureka_nodes_.end())
		return false;
	EurekaNodeInfo& ni = fiter->second;
	return ni.is_same_node(token);
}

void EurekaClusterCtrl::on_disconnected_with_linkto(EurekaLinkTo* plink)
{
	eureka_links_to_.on_linkto_disconnected(plink);
}

void EurekaClusterCtrl::on_linkto_regist_result(EurekaLinkTo* plink)
{
	eureka_links_to_.on_linkto_regist_result(plink);
}

void EurekaClusterCtrl::on_disconnected_with_linkfrom(EurekaLinkFrom* plink)
{
	//结合sEurekaApp的disconnect处理流程
	eureka_links_from_.return_freelink(plink);
}
