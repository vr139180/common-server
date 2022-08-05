#include "lobby/user/UserBuildings.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

using namespace google::protobuf;

USED_REDISKEY_USER_NS

UserBuildings::UserBuildings():building_data_update_(false)
, dels_update_(false)
{

}

void UserBuildings::reset_data()
{
	building_data_update_ = false;
	building_data_.Clear();

	dels_update_ = false;
	dels_.Clear();
}

void UserBuildings::data_copyall(UserBuildings& from)
{
	data_copy_from<PRO::DBUserHomeStructure>(&from.building_data_);
	datadels_copy_from<PRO::DBRowDeletes>(&from.dels_);
}

void UserBuildings::load_from_database(sql::ResultSet& row)
{
	building_data_update_ = false;
	building_data_.Clear();

	dels_update_ = false;
	dels_.Clear();

	while (row.next())
	{
		PRO::DBUserHomeStructureItem *pitem = building_data_.add_items();

		int column = 1;
		pitem->set_ver_(row.getUInt(column++));

		pitem->set_building_iid(row.getInt64(column++));
		pitem->set_home_iid(row.getInt64(column++));
		pitem->set_parent_building(row.getInt64(column++));
		pitem->set_building_resid(row.getInt64(column++));
		pitem->set_look_at(row.getString(column++).c_str());
		pitem->set_building_pos(row.getString(column++).c_str());
		pitem->set_levels(row.getInt(column++));
	}
}

void UserBuildings::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	building_data_update_ = false;
	building_data_.Clear();

	rdv->get_hashobject(key.c_str(), USER_DETAIL_BUILDING, &building_data_);

	dels_update_ = false;
	dels_.Clear();

	rdv->get_hashobject(key.c_str(), USER_DETAIL_BUILDING_DEL, &dels_);
}

bool UserBuildings::update_redis_datacache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!building_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_BUILDING, &building_data_, svrApp.get_redisprotocache()))
	{
		building_data_update_ = false;
		return true;
	}

	return false;
}

bool UserBuildings::update_redis_delscache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!dels_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_BUILDING_DEL, &dels_, svrApp.get_redisprotocache()))
	{
		dels_update_ = false;
		return true;
	}
	return false;
}

bool UserBuildings::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	update_redis_datacache(roleid, rdv, force);
	update_redis_delscache(roleid, rdv, force);

	return true;
}

PRO::DBUserHomeStructureItem* UserBuildings::add_item()
{
	building_data_update_ = true;
	return building_data_.add_items();
}

bool UserBuildings::del_item(S_INT_64 iid, S_UINT_32 ver)
{
	bool exist = false;
	RepeatedPtrField<PRO::DBUserHomeStructureItem>* builds = building_data_.mutable_items();
	for (RepeatedPtrField<PRO::DBUserHomeStructureItem>::const_iterator iter = builds->begin(); iter != builds->end(); )
	{
		const PRO::DBUserHomeStructureItem& it = (*iter);
		if (it.building_iid() == iid)
		{
			exist = true;
			builds->erase(iter);
			break;
		}
	}

	if (exist)
	{
		building_data_update_ = true;

		PRO::DBRowDel* dd = dels_.add_dels();
		dd->set_id(iid);
		dd->set_ver_(ver);

		dels_update_ = true;
	}

	return exist;
}
