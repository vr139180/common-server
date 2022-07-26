package userdata

import (
	"cmslib/redisutil"

	"google.golang.org/protobuf/proto"
	"xorm.io/xorm"
)

type IUserDataModule interface {
	LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool

	BuildValidData(dbver uint32, cachever uint32)

	SaveDatabase(session *xorm.Session) error
}

//--------------------------------------------------------------
type userOperator struct {
}

func (u *userOperator) stringToMessage(md map[string]string, key string, msg proto.Message) bool {
	bd, ok := md[key]
	if !ok {
		return false
	}

	d1 := []byte(bd)

	return proto.Unmarshal(d1, msg) == nil
}

func (u *userOperator) stringToMessageIgnore(md map[string]string, key string, msg proto.Message) bool {
	bd, ok := md[key]
	if !ok {
		return true
	}

	d1 := []byte(bd)

	return proto.Unmarshal(d1, msg) == nil
}

func (u *userOperator) LoadFromRedis(mdata map[string]string, r *redisutil.RedisUtil) bool {
	return false
}

func (u *userOperator) BuildValidData(dbver uint32, cachever uint32) {
}

func (u *userOperator) SaveDatabase(session *xorm.Session) error {
	return nil
}

func (u *userOperator) InsertOrUpdate(session *xorm.Session, id interface{}, bean interface{}) error {

	n, err := session.Update(bean)
	if err != nil {
		return err
	}

	if n <= 0 {
		ok, err := session.Exist(id)
		if err != nil {
			return err
		}

		if !ok {
			_, err = session.Insert(bean)
			if err != nil {
				return err
			}
		}
	}

	return nil
}
