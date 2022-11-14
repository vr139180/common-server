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

#include "lobby/user/UserCacheData.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>

#include "dbs/DBSCtrl.h"

#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

PRO::DBUserHomeStructureItem* UserCacheData::build_additem(S_INT_64 parent, S_INT_64 resid, 
	const char* lookat, const char* pos, S_INT_32& ret)
{
	//message result
	ret = 0;

	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);
	S_INT_64 iid = dbsStore->gen_dbid(DBID_USER_HOME_STRUCTURE, rdv);

	PRO::DBUserHomeStructureItem* pitem = building_data_.add_item();
	pitem->set_ver_(newver);
	pitem->set_building_iid(iid);
	pitem->set_home_iid(home_data_.get_home_iid());
	pitem->set_parent_building(parent);
	pitem->set_building_resid(resid);
	pitem->set_look_at(lookat);
	pitem->set_building_pos(pos);
	pitem->set_levels(1);

	this->save_all(false, rdv);

	return pitem;
}

S_INT_32 UserCacheData::build_delitem(S_INT_64 buildiid)
{
	S_INT_32 ret = 0;

	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);
	if (!building_data_.del_item(buildiid, newver))
		ret = 1;
	else
		save_all(false, rdv);

	return ret;
}
