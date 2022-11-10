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
#include <gameLib/LogExt.h>

#include "sEurekaApp.h"
#include "debugdefine.h"

USED_REDISKEY_GLOBAL_NS

bool EurekaClusterCtrl::update_redis_masterinfo(RedisClient* rdv)
{
	if( rdv == 0)
		rdv = svrApp.get_redisclient();

	if (!rdv->set_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, myself_.iid))
		return false;
	if (!rdv->set_hashobject(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_SVR, myself_))
		return false;

	//rdv->pexpire(EUREKA_MASTER_NODE, EUREKA_MASTER_NODE_TIMEOUT);
	
	return true;
}
