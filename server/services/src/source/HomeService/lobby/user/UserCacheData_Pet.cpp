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

PRO::DBUserPetItem* UserCacheData::pet_adoptone(S_INT_64 resid, S_INT_32& ret)
{
	//message result
	ret = 0;

	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);
	S_INT_64 iid = dbsStore->gen_dbid(DBID_USER_PETS, rdv);

	PRO::DBUserPetItem* pitem = pet_data_.adopt_one();
	pitem->set_ver_(newver);
	pitem->set_mypet_iid(iid);

	pitem->set_role_iid( role_iid_);
	pitem->set_pet_iid( resid);
	pitem->set_pet_age(10);
	pitem->set_birthday(OSSystem::mOS->GetUnixTimestamp());

	this->save_all(false, rdv);

	return pitem;
}

S_INT_32 UserCacheData::pet_releaseone(S_INT_64 mypetiid)
{
	S_INT_32 ret = 0;

	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);
	if (!pet_data_.release_one(mypetiid, newver))
		ret = 1;
	else
		save_all(false, rdv);

	return ret;
}
