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

type userBase struct {
	*userOperator

	base_data gpro.DBRoleBaseInfo

	is_modify bool
}

func (u *userBase) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	u.is_modify = false
	return u.stringToMessage(mdata, redis.USER_DETAIL_USERBASE, &u.base_data)
}

func (u *userBase) BuildValidData(dbver uint32, cachever uint32) {
	//****更新role的数据库版本号
	u.base_data.Ver_ = cachever
	u.is_modify = true
}

func (u *userBase) SaveDatabase(session *xorm.Session) error {
	if !u.is_modify {
		return nil
	}

	bean := u.redisBean2DbBean()
	_, err := session.ID(bean.RoleIid).Update(bean)

	return err
}

func (u *userBase) redisBean2DbBean() (b *entity.RoleBaseInfo) {
	b = &entity.RoleBaseInfo{}
	b.Ver_ = u.base_data.GetVer_()

	b.RoleIid = u.base_data.GetRoleIid()
	b.UserIid = u.base_data.GetUserIid()
	b.Nickname = u.base_data.GetNickname()
	b.Registime = utilc.SecondToTime(u.base_data.GetRegistime())
	b.Levels = u.base_data.GetLevels()
	pos := u.base_data.GetLoc()
	b.LocX = pos.X
	b.LocY = pos.Y
	b.LocZ = pos.Z

	return b
}

//---------------------------------------------------------------------------------------------
type userHome struct {
	*userOperator

	home_data gpro.DBUserHome

	is_modify bool
}

func (u *userHome) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	u.is_modify = false
	return u.stringToMessage(mdata, redis.USER_DETAIL_USERHOME, &u.home_data)
}

func (u *userHome) BuildValidData(dbver uint32, cachever uint32) {
	if u.home_data.GetVer_() > dbver {
		u.is_modify = true
	}
}

func (u *userHome) SaveDatabase(session *xorm.Session) error {
	if !u.is_modify {
		return nil
	}

	bean := u.redisBean2DbBean()
	idbean := &entity.UserHome{RoleIid: bean.RoleIid}

	err := u.InsertOrUpdate(session, idbean, bean)

	return err
}

func (u *userHome) redisBean2DbBean() (b *entity.UserHome) {
	b = &entity.UserHome{}
	b.Ver_ = u.home_data.GetVer_()

	b.RoleIid = u.home_data.GetRoleIid()
	b.HomeName = u.home_data.GetHomeName()
	b.GroundResId = u.home_data.GetGroundResid()
	b.LookAt = u.home_data.GetLookAt()
	b.GeoPos = u.home_data.GetGeoPos()
	b.ResideTime = u.home_data.GetResideTime()
	b.LastResideDate = utilc.SecondToTime(u.home_data.GetLastResidedate())
	b.Levels = u.home_data.GetLevels()

	return b
}
