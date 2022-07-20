#include "lobby/user/UserCacheData.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>

#include "dbs/DBSCtrl.h"
#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

void UserCacheData::task_new_group(TaskGroupCellRT* gcrt)
{
	RedisClient* rdv = svrApp.get_redisclient();

	S_UINT_32 newver = this->inc_cachever(rdv);

	PRO::DBUserTaskGroup* pGroup = task_data_.new_taskgroup();
	pGroup->set_ver_(newver);

	pGroup->set_iid(gcrt->get_iid());

	pGroup->set_role_iid(this->role_iid_);
	pGroup->set_task_group(gcrt->get_groupiid());
	pGroup->set_group_cell(gcrt->get_groupcell_iid());
	pGroup->set_gstate( (S_INT_32)gcrt->get_gstate());

	//TODO:level
	pGroup->set_trigg_level(0);
	pGroup->set_createtime(OSSystem::mOS->GetUnixTimestamp());
	pGroup->set_endtime(0);

	//cell_data
	google::protobuf::Map<S_INT_32, S_INT_32>& pData = *(pGroup->mutable_cell_data()->mutable_datas());
	const boost::unordered_map<S_INT_32, PRO::TASK_STATE>& celldata = gcrt->get_celldata();
	for (boost::unordered_map<S_INT_32, PRO::TASK_STATE>::const_iterator citer = celldata.begin(); citer != celldata.end(); ++citer)
	{
		pData[citer->first] = (S_INT_32)citer->second;
	}

	this->save_all(false, rdv);
}
