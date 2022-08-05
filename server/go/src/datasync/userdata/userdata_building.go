package userdata

import (
	"cmslib/redisutil"
	"datasync/db/entity"
	"gamelib/protobuf/gpro"
	"gamelib/redis"

	"xorm.io/xorm"
)

type userBuildings struct {
	*userOperator

	building_data gpro.DBUserHomeStructure
	dels_data     gpro.DBRowDeletes

	b_modify bool
}

func (u *userBuildings) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	u.b_modify = false

	if !u.stringToMessage(mdata, redis.USER_DETAIL_BUILDING, &u.building_data) {
		return false
	}

	if !u.stringToMessage(mdata, redis.USER_DETAIL_BUILDING_DEL, &u.dels_data) {
		return false
	}

	return true
}

func (u *userBuildings) BuildValidData(dbver uint32, cachever uint32) {

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
	dats := make([]*gpro.DBUserHomeStructureItem, 0)
	for _, d1 := range u.building_data.Items {
		if d1.GetVer_() > dbver {
			u.b_modify = true
			dats = append(dats, d1)
		}
	}
	u.building_data.Items = dats
}

func (u *userBuildings) SaveDatabase(session *xorm.Session) error {
	if !u.b_modify {
		return nil
	}

	//-------------------先做 insert update------------------------
	for _, d1 := range u.building_data.Items {
		bean := u.redisBean2DbBean(d1)
		idbean := &entity.UserHomeStructure{BuildingIid: d1.BuildingIid}

		err := u.InsertOrUpdate(session, idbean, bean)
		if err != nil {
			return err
		}
	}

	//-----------------------dels 时需要判断version---------------------
	for _, d2 := range u.dels_data.Dels {

		_, err := session.Where("ver_ < ?", d2.GetVer_()).Delete(&entity.UserHomeStructure{BuildingIid: d2.GetId()})
		if err != nil {
			return err
		}
	}

	return nil
}

func (u *userBuildings) redisBean2DbBean(dat *gpro.DBUserHomeStructureItem) (b *entity.UserHomeStructure) {
	b = &entity.UserHomeStructure{}
	b.Ver_ = dat.GetVer_()

	b.BuildingIid = dat.GetBuildingIid()
	b.HomeIid = dat.GetHomeIid()
	b.ParentBuilding = dat.GetParentBuilding()
	b.BuildingResId = dat.GetBuildingResid()
	b.LookAt = dat.GetLookAt()
	b.BuildingPos = dat.GetBuildingPos()
	b.Levels = dat.GetLevels()

	return b
}
