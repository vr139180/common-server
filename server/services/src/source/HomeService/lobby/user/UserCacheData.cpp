#include "lobby/user/UserCacheData.h"

#include <cmsLib/redis/RedisClient.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

USED_REDISKEY_USER_NS

UserCacheData::UserCacheData():user_iid_(-1)
, slottoken_(0)
, giduid_(0)
{
}

void UserCacheData::reset_usercache()
{
	this->user_iid_ = -1;
	this->slottoken_ = 0;
	this->giduid_ = 0;

	base_data_.reset_data();
	home_data_.reset_data();
	building_data_.reset_data();
	pet_data_.reset_data();

	task_resolver_.reset_resolver();
	task_data_.reset_data();
}

/*
* 从数据构建初始版本控制信息，
* 或者从数据库同步数据再次构建增量控制信息
*/
void UserCacheData::on_db_roledata_sync1( UserBase& ubase, UserHome& uhome, 
	UserBuildings& ubuilds, UserPets& upets, UserTasks& utasks)
{
	base_data_.reset_data();
	home_data_.reset_data();
	building_data_.reset_data();
	pet_data_.reset_data();
	task_data_.reset_data();

	this->base_data_.data_copyall(ubase);
	this->home_data_.data_copyall(uhome);
	this->building_data_.data_copyall(ubuilds);
	this->pet_data_.data_copyall(upets);
	
	this->task_data_.data_copyall(utasks);
	this->task_data_loaded();

	//初始版本信息
	this->db_ver_ = base_data_.data().ver_();
	this->cache_ver_ = this->db_ver_;

	this->save_all( true);

	//初始化一次版本信息
	update_dbver_redis_cache();
	update_cachever_redis_cache(cache_ver_);
}

bool UserCacheData::sync_all()
{
	RedisClient* rdv = svrApp.get_redisclient();
	if (!active_14days( rdv))
		return false;

	base_data_.load_from_redis(role_iid_, rdv);
	home_data_.load_from_redis(role_iid_, rdv);
	building_data_.load_from_redis(role_iid_, rdv);
	pet_data_.load_from_redis(role_iid_, rdv);
	task_data_.load_from_redis(role_iid_, rdv);
	this->task_data_loaded();

	db_ver_ = get_dbver_from_redis(rdv);
	cache_ver_ = get_cachever_from_redis(rdv);

	return true;
}

bool UserCacheData::active_14days(RedisClient* rdv)
{
	return base_data_.active_14days(role_iid_, rdv);
}

void UserCacheData::save_all(bool force, RedisClient* rdv)
{
	if( rdv == 0)
		rdv = svrApp.get_redisclient();

	//先同步redis的db version，根据db ver更新dels
	bool bmodify = false;

	if (!force)
	{
		db_ver_ = get_dbver_from_redis(rdv);
		
		base_data_.adjust_dels(db_ver_);

		home_data_.adjust_dels(db_ver_);
		building_data_.adjust_dels(db_ver_);
		pet_data_.adjust_dels(db_ver_);

		if (base_data_.is_modify())
			bmodify = true;
		else if (home_data_.is_modify())
			bmodify = true;
		else if (building_data_.is_modify())
			bmodify = true;
		else if (pet_data_.is_modify())
			bmodify = true;
		else if (task_data_.is_modify())
			bmodify = true;
	}

	base_data_.update_redis_cache( role_iid_, rdv, force);

	home_data_.update_redis_cache(role_iid_, rdv, force);
	building_data_.update_redis_cache(role_iid_, rdv, force);
	pet_data_.update_redis_cache(role_iid_, rdv, force);
	task_data_.update_redis_cache(role_iid_, rdv, force);

	if (bmodify)
		mark_datasync(rdv);
}

S_INT_64 UserCacheData::get_dbver_from_redis(RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(USER_DETAIL, role_iid_);
	
	S_INT_64 v = 0;
	rdv->get_hashmember_ul(key.c_str(), USER_DETAIL_VERSION_DB, v);
	return v;
}

S_INT_64 UserCacheData::get_cachever_from_redis(RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(USER_DETAIL, role_iid_);

	S_INT_64 v = 0;
	rdv->get_hashmember_ul(key.c_str(), USER_DETAIL_VERSION_RD, v);
	return v;
}

bool UserCacheData::update_dbver_redis_cache(RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(USER_DETAIL, role_iid_);
	return rdv->set_hashmember_ul(key.c_str(), USER_DETAIL_VERSION_DB, db_ver_);
}

bool UserCacheData::update_cachever_redis_cache(S_INT_64 ver, RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(USER_DETAIL, role_iid_);
	return rdv->set_hashmember_ul(key.c_str(), USER_DETAIL_VERSION_RD, ver);
}

S_UINT_32 UserCacheData::inc_cachever(RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	std::string key = rdv->build_rediskey(USER_DETAIL, role_iid_);
	rdv->inc_hashmemeber(key.c_str(), USER_DETAIL_VERSION_RD, cache_ver_);

	return (S_UINT_32)cache_ver_;
}

void UserCacheData::mark_datasync(RedisClient* rdv)
{
	if (rdv == 0)
		rdv = svrApp.get_redisclient();

	rdv->add_zset(DATASYNC_USERS, std::to_string(role_iid_).c_str(), OSSystem::mOS->GetTimestamp(), UpdateType::NOT_EXIST);
}
