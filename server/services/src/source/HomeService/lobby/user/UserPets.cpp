#include "lobby/user/UserPets.h"

#include <gameLib/redis/user_redis_const.h>

#include "HomeServiceApp.h"

using namespace google::protobuf;

USED_REDISKEY_USER_NS

UserPets::UserPets():pets_data_update_(false)
,dels_update_(false)
{
}

void UserPets::reset_data()
{
	pets_data_.Clear();
	pets_data_update_ = false;

	dels_.Clear();
	dels_update_ = false;
}

void UserPets::data_copyall(UserPets& from)
{
	data_copy_from<PRO::DBUserPets>(&from.pets_data_);
	datadels_copy_from<PRO::DBRowDeletes>(&from.dels_);
}

void UserPets::load_from_database(sql::ResultSet& row)
{
	pets_data_.Clear();
	pets_data_update_ = false;

	dels_.Clear();
	dels_update_ = false;

	while (row.next())
	{
		PRO::DBUserPetItem *pitem = pets_data_.add_pets();

		int column = 1;
		pitem->set_ver_(row.getUInt(column++));

		pitem->set_mypet_iid(row.getInt64(column++));
		pitem->set_role_iid(row.getInt64(column++));
		pitem->set_pet_iid(row.getInt64(column++));
		pitem->set_pet_age(row.getInt(column++));
		pitem->set_birthday(row.getInt(column++));
	}
}

void UserPets::load_from_redis(S_INT_64 roleid, RedisClient* rdv)
{
	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	pets_data_.Clear();
	pets_data_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_PET, &pets_data_);

	dels_.Clear();
	dels_update_ = false;

	rdv->get_hashobject(key.c_str(), USER_DETAIL_PET_DEL, &dels_);
}

bool UserPets::update_redis_datacache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!pets_data_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_PET, &pets_data_, svrApp.get_redisprotocache()))
	{
		pets_data_update_ = false;
		return true;
	}
	return true;
}

bool UserPets::update_redis_delscache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	if (!force)
	{
		if (!dels_update_)
			return true;
	}

	std::string key = rdv->build_rediskey(USER_DETAIL, roleid);

	if (rdv->set_hashobject(key.c_str(), USER_DETAIL_PET_DEL, &dels_, svrApp.get_redisprotocache()))
	{
		dels_update_ = false;
		return true;
	}
	return false;
}

bool UserPets::update_redis_cache(S_INT_64 roleid, RedisClient* rdv, bool force)
{
	update_redis_datacache(roleid, rdv, force);
	update_redis_delscache(roleid, rdv, force);

	return true;
}

PRO::DBUserPetItem* UserPets::adopt_one()
{
	pets_data_update_ = true;
	return pets_data_.add_pets();
}

bool UserPets::release_one(S_INT_64 mypetiid, S_UINT_32 ver)
{
	bool exist = false;
	RepeatedPtrField<PRO::DBUserPetItem>* pets = pets_data_.mutable_pets();
	for (RepeatedPtrField<PRO::DBUserPetItem>::const_iterator iter = pets->begin(); iter != pets->end(); )
	{
		const PRO::DBUserPetItem& it = (*iter);
		if (it.mypet_iid() == mypetiid)
		{
			exist = true;
			pets->erase(iter);
			break;
		}
	}

	if (exist)
	{
		pets_data_update_ = true;

		PRO::DBRowDel* dd = dels_.add_dels();
		dd->set_id(mypetiid);
		dd->set_ver_(ver);

		dels_update_ = true;
	}

	return exist;
}
