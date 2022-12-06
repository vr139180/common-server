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

package dbserialize

import (
	"cmslib/logx"
	"cmslib/redisutil"
	"datasync/g"
	"datasync/userdata"
	"gamelib/redis"
)

func (l *serializeLoop) processSerializeCmd(roleid string) {
	l.dbcmd <- roleSerializeCmd{roleiid: roleid}
}

func (l *serializeLoop) doRoleService(roleid string) {
	redisclient := g.GetRedis()

	ud := userdata.NewUserData(roleid, redisclient)
	if ud == nil {
		logx.Errorf("datasync service load role:%s from redis failed", roleid)
		return
	}

	dbclient := g.DBS()
	if ud.SaveToDatabase(dbclient, redisclient) {
		l.SaveConfirm(roleid, ud.GetCacheVer(), redisclient)
	}
}

/**
*key:
USER_DETAIL,DATASYNC_USERS_CONFIRM
*values:
roleid, dbver
*result
0:succ 1:user online
**/
const REDIS_DATASYNC_CONFIRM_SC = `
redis.call('HSET', KEYS[1],'ver:db', ARGV[2])
redis.call('SREM', KEYS[2], ARGV[1])
return {'0'}
`

func (l *serializeLoop) SaveConfirm(roleid string, ver uint32, r *redisutil.RedisUtil) {

	key := r.BuildKey(redis.USER_DETAIL, roleid)
	keys := []string{key, redis.DATASYNC_USERS_CONFIRM}

	_, err := r.Eval(REDIS_DATASYNC_CONFIRM_SC, keys, roleid, ver)
	if err != nil {
		logx.Errorf("datasync SaveConfirm failed, redis script result:%s", err.Error())
	}
}
