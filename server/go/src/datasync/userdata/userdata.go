package userdata

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/redisutil"
	"gamelib/redis"
	"strconv"

	"xorm.io/xorm"
)

type UserData struct {
	base_data     userBase
	home_data     userHome
	building_data userBuildings
	pets_data     userPets
	cache_ver     uint32
	db_ver        uint32

	all_datas []IUserDataModule

	need_save bool

	role_iid string
}

func NewUserData(roleid string, rdv *redisutil.RedisUtil) (u *UserData) {
	u = new(UserData)
	u.role_iid = roleid
	u.dataModuls()

	if !u.loadFromRedis(rdv) {
		return nil
	}

	return u
}

func (u *UserData) dataModuls() {
	u.registOneModule(&u.base_data)
	u.registOneModule(&u.home_data)
	u.registOneModule(&u.building_data)
	u.registOneModule(&u.pets_data)
}

func (u *UserData) GetCacheVer() uint32 {
	return u.cache_ver
}

func (u *UserData) registOneModule(mod interface{}) {
	m := mod.(IUserDataModule)
	u.all_datas = append(u.all_datas, m)
}

func (u *UserData) loadFromRedis(r *redisutil.RedisUtil) bool {

	key := r.BuildKey(redis.USER_DETAIL, u.role_iid)
	mdata, err := r.HGetAll(key)
	if err != nil {
		return false
	}

	for _, val := range u.all_datas {
		if !val.LoadFromRedis(mdata, r) {
			return false
		}
	}

	str, ok := mdata[redis.USER_DETAIL_VERSION_RD]
	if !ok {
		return false
	}

	var v1 int64
	//cache and db version from redis
	v1, err = strconv.ParseInt(str, 10, 64)
	if err != nil {
		return false
	}
	u.cache_ver = uint32(v1)

	str, ok = mdata[redis.USER_DETAIL_VERSION_DB]
	if !ok {
		return false
	}
	v1, err = strconv.ParseInt(str, 10, 64)
	if err != nil {
		return false
	}
	u.db_ver = uint32(v1)

	u.need_save = (u.cache_ver > u.db_ver)

	//根据版本信息过滤不需要处理的数据
	//记录中ver大于db版本，表示是需要更新的记录
	for _, val := range u.all_datas {
		val.BuildValidData(u.db_ver, u.cache_ver)
	}

	return true
}

func (u *UserData) SaveToDatabase(db *mysqlx.MysqlClient, rdv *redisutil.RedisUtil) bool {
	if !u.need_save {
		return true
	}

	_, err := db.Transaction(func(session *xorm.Session) (interface{}, error) {
		err := session.Begin()
		if err != nil {
			return nil, err
		}

		for _, val := range u.all_datas {
			err = val.SaveDatabase(session)
			if err != nil {
				break
			}
		}

		if err != nil {
			session.Rollback()
			return nil, err
		}

		err = session.Commit()

		return nil, err
	})

	if err != nil {
		logx.Errorf("role:%s serialize to database failed, err:%s", u.role_iid, err.Error())
		return false
	}

	return true
}
