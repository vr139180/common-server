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
