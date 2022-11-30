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

#ifndef __USERCACHEDATA_H__
#define __USERCACHEDATA_H__

#include <boost/dynamic_bitset.hpp>

#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include <taskLib/service/ITaskEnv.h>
#include <taskLib/service/TaskGroupCellRT.h>
#include <taskLib/service/TaskCellRT.h>
#include <taskLib/UserTasksResolver.h>

#include "lobby/user/UserBuildings.h"
#include "lobby/user/UserPets.h"
#include "lobby/user/UserBase.h"
#include "lobby/user/UserHome.h"
#include "lobby/user/UserTasks.h"

class LobbyUser;

class UserCacheData : public ITaskDataUpdateCB
{
	friend class LobbyUser;

public:
	UserCacheData();

	void reset_usercache();

	virtual S_INT_64 get_user_iid() = 0;

	S_INT_64 get_role_iid() { return role_iid_; }
	void set_role_iid(S_INT_64 rid) { role_iid_ = rid; }

	S_UINT_32 inc_cachever(RedisClient* rdv = 0);
	S_INT_64 get_dbver_from_redis(RedisClient* rdv = 0);
	S_INT_64 get_cachever_from_redis(RedisClient* rdv = 0);
	bool update_dbver_redis_cache(RedisClient* rdv = 0);
	bool update_cachever_redis_cache(S_INT_64 ver, RedisClient* rdv = 0);

public:
	//base
	const UserBase& get_user_baseinfo() { return base_data_; }

public:
	//building
	/*
	* @parent	父建筑id, =0直接建筑在基地
	* @resid	资源id
	* @lookat	朝向
	*/
	PRO::DBUserHomeStructureItem* build_additem(S_INT_64 parent, S_INT_64 resid, 
		const char* lookat, const char* pos, S_INT_32& ret);
	S_INT_32 build_delitem(S_INT_64 buildiid);

public:
	//pet
	PRO::DBUserPetItem* pet_adoptone(S_INT_64 resid, S_INT_32& ret);
	S_INT_32 pet_releaseone(S_INT_64 mypetiid);

public:
	//-------------------------implement ITaskDataUpdateCB -------------------------
	virtual void notify_new_taskgroup(TaskGroupCellRT* gcrt);
	virtual void notify_forward_nextcell_taskgroup(TaskGroupCellRT* gcrt);
	virtual void notify_taskgroup_end(TaskGroupCellRT* gcrt);
	virtual void notify_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc);
	virtual void notify_end_task(TaskGroupCellRT* gcrt, TaskCellRT* tc);
	virtual void notify_get_taskreward(TaskCellRT* tc, TaskRewardMeta* reward);

	//tasks
	virtual void task_data_loaded() {}

	void task_new_group(TaskGroupCellRT* gcrt);
	void task_group_nextcell(TaskGroupCellRT* gcrt);
	void task_group_end(TaskGroupCellRT* gcrt);
	void task_user_gettask(TaskGroupCellRT* gcrt, TaskCellRT* tc);
	void task_user_submittask(TaskGroupCellRT* gcrt, TaskCellRT* tc);
	void task_user_getrewards(TaskCellRT* tc, TaskRewardMeta* reward);

	BasicProtocol* task_get_waitlist();
	BasicProtocol* task_get_mytasks();
	BasicProtocol* task_get_from_waitlist(S_INT_32 taskid);
	BasicProtocol* task_submit_one(S_INT_32 taskid);
	BasicProtocol* task_giveup_task(S_INT_32 taskid);

public:
	//同步数据
	void on_db_roledata_sync1( UserBase& ubase, UserHome& uhome, 
		UserBuildings& ubuilds, UserPets& upets, UserTasks& utasks);

protected:
	//sync first
	//ret: false -> need to load data from database and save to redis
	bool sync_all();
	void save_all( bool force = false, RedisClient* rdv = 0);
	//14天过期时间设置
	bool active_14days( RedisClient* rdv);

	//mark user need to sync to database
	void mark_datasync(RedisClient* rdv);

protected:
	UserBase		base_data_;
	UserHome		home_data_;
	UserBuildings	building_data_;
	UserPets		pet_data_;
	UserTasks		task_data_;

protected:
	UserTasksResolver	task_resolver_;

protected:
	//当前role iid
	S_INT_64	role_iid_;

protected:
	//cache中的版本信息
	S_INT_64	cache_ver_;
	//db的版本信息
	S_INT_64	db_ver_;
};

#endif //__USERCACHEDATA_H__
