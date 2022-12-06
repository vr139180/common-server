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

package userdata

import (
	"cmslib/redisutil"
	"cmslib/utilc"
	"datasync/db/entity"
	"gamelib/protobuf/gpro"
	"gamelib/redis"

	"xorm.io/xorm"
)

type userPets struct {
	*userOperator

	pets_data gpro.DBUserPets
	dels_data gpro.DBRowDeletes

	b_modify bool
}

func (u *userPets) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	u.b_modify = false

	if !u.stringToMessage(mdata, redis.USER_DETAIL_PET, &u.pets_data) {
		return false
	}

	if !u.stringToMessage(mdata, redis.USER_DETAIL_PET_DEL, &u.dels_data) {
		return false
	}

	return true
}

func (u *userPets) BuildValidData(dbver uint32, cachever uint32) {
	//先过滤 dels
	dels := make([]*gpro.DBRowDel, 0)
	for _, d2 := range u.dels_data.Dels {
		if d2.GetVer_() > dbver {
			u.b_modify = true
			dels = append(dels, d2)
		}
	}
	u.dels_data.Dels = dels

	//--------------------------------------------------------------
	dats := make([]*gpro.DBUserPetItem, 0)
	for _, d1 := range u.pets_data.Pets {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			dats = append(dats, d1)
		}
	}
	u.pets_data.Pets = dats
}

func (u *userPets) SaveDatabase(session *xorm.Session) error {
	if !u.b_modify {
		return nil
	}

	//-------------------先做 insert update------------------------
	for _, d1 := range u.pets_data.Pets {
		bean := u.redisBean2DbBean(d1)
		idbean := &entity.UserPet{MyPetIid: d1.GetMypetIid()}

		err := u.InsertOrUpdate(session, idbean, bean)
		if err != nil {
			return err
		}
	}

	//-----------------------dels 时需要判断version---------------------
	for _, d2 := range u.dels_data.Dels {

		_, err := session.Where("ver_ < ?", d2.GetVer_()).Delete(&entity.UserPet{MyPetIid: d2.GetId()})
		if err != nil {
			return err
		}
	}

	return nil
}

func (u *userPets) redisBean2DbBean(dat *gpro.DBUserPetItem) (b *entity.UserPet) {
	b = &entity.UserPet{}
	b.Ver_ = dat.GetVer_()

	b.MyPetIid = dat.GetMypetIid()
	b.RoleIid = dat.GetRoleIid()
	b.PetIid = dat.GetPetIid()
	b.PetAge = dat.GetPetAge()
	b.Birthday = utilc.SecondToTime(dat.GetBirthday())

	return b
}
