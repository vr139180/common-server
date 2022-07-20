#ifndef __USERCACHEDATA_H__
#define __USERCACHEDATA_H__

#include <boost/dynamic_bitset.hpp>

#include <cmsLib/redis/RedisClient.h>
#include <cmsLib/prolib/core_type.h>
#include <gameLib/protobuf/cpp/db_internal.pb.h>

#include <taskLib/service/TaskGroupCellRT.h>
#include <taskLib/service/TaskCellRT.h>

#include "lobby/user/UserBuildings.h"
#include "lobby/user/UserPets.h"
#include "lobby/user/UserBase.h"
#include "lobby/user/UserHome.h"
#include "lobby/user/UserTasks.h"

class LobbyUser;

class UserCacheData
{
	friend class LobbyUser;

public:
	UserCacheData();

	void reset_usercache();

	S_INT_64 get_user_iid() { return user_iid_; }
	void set_user_iid(S_INT_64 uid) { this->user_iid_ = uid; }
	S_INT_64 get_slottoken() { return slottoken_; }
	void set_slottoken(S_INT_64 t) { this->slottoken_ = t; }
	S_INT_64 get_giduid() { return giduid_; }
	void set_giduid(S_INT_64 v) { this->giduid_ = v; }
	S_INT_64 get_role_iid() { return role_iid_; }
	void set_role_iid(S_INT_64 rid) { this->role_iid_ = rid; }

	S_UINT_32 inc_cachever(RedisClient* rdv = 0);
	S_INT_64 get_dbver_from_redis(RedisClient* rdv = 0);
	S_INT_64 get_cachever_from_redis(RedisClient* rdv = 0);
	bool update_dbver_redis_cache(RedisClient* rdv = 0);
	bool update_cachever_redis_cache(S_INT_64 ver, RedisClient* rdv = 0);

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
	//tasks
	virtual void task_data_loaded() {}

	void task_new_group(TaskGroupCellRT* gcrt);

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
	//当前role iid
	S_INT_64	role_iid_;
	S_INT_64	user_iid_;
	//proto token
	S_INT_64	slottoken_;
	S_INT_64	giduid_;

protected:
	//cache中的版本信息
	S_INT_64	cache_ver_;
	//db的版本信息
	S_INT_64	db_ver_;
};

#endif //__USERCACHEDATA_H__
