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
