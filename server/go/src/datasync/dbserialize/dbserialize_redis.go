package dbserialize

import (
	"cmslib/logx"
	"cmslib/utilc"
	"datasync/g"
	"gamelib/redis"
)

/**
*key:
DATASYNC_USERS,DATASYNC_USERS_CONFIRM
*values:
nowtimestamp,nums
*result
0:succ 1:user online
**/
const REDIS_DATASYNC_SC = `
local tnow = tonumber(ARGV[1])
local num = tonumber(ARGV[2])
local lt = redis.call('ZRANGEBYSCORE', KEYS[1], 0, tnow, 'LIMIT',0, num)
for i, v in ipairs(lt)
do
	redis.call('ZREM', KEYS[1], v)
	redis.call('SADD', KEYS[2], v)
end
return lt
`

func (r *DBSerializePool) timer_roleserialize_queue(num int) {
	redisclient := g.GetRedis()

	keys := []string{redis.DATASYNC_USERS, redis.DATASYNC_USERS_CONFIRM}

	//5 mins
	modify_delay := 5 * 60 * 1000
	//modify_delay := 5 * 1000

	//检查 用户缓存，是否在线
	d := utilc.GetTimestamp()
	lastupdate := d - int64(modify_delay)
	dat, err := redisclient.Eval(REDIS_DATASYNC_SC, keys, lastupdate, num)
	if err != nil {
		logx.Errorf("datasync get rolelist redis script error:%s", err.Error())
	} else {
		logx.Debugf("datasync will process role serizied cmd:%d", len(dat))
		for _, iid := range dat {
			l := r.dbCmdPool()
			l.processSerializeCmd(iid)
		}
	}
}
