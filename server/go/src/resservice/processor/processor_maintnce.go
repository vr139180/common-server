package processor

import (
	"cmslib/logx"
	"cmslib/utilc"
	"fmt"
	"gamelib/redis"
	"resservice/g"
)

func (p *NetProcessor) DoUserSlotAuthMaintnce() {
	logx.Debugf(".....to do , gate slot auth maintnce..............")

	redisclient := g.GetRedis()
	tnow := utilc.GetTimestamp() - 5000

	ls, err := redisclient.GetZsetAllMOfKey(redis.HOMEGATE_WATERSLOT)
	if err == nil && len(ls) > 0 {
		for _, v := range ls {
			skey := redisclient.BuildKey(redis.HOMEGATE_USERSLOT_AUTH, v)
			dn, err2 := redisclient.DelZsetLessThen(skey, tnow)
			if err2 != nil && dn > 0 {
				logx.Errorf("res slot maintnce timeout check _HOMEGATE:waterslot key:%s nums:%d", skey, dn)
			} else if err2 != nil {
				logx.Errorf("res slot maintnce timeout check _HOMEGATE:waterslot DelZsetLessThen error:%s", err2.Error())
			}
		}
	}

	ls, err = redisclient.GetZsetAllMOfKey(redis.HOMEGATE_FLEXSLOT)
	if err == nil && len(ls) > 0 {
		for _, v := range ls {
			skey := redisclient.BuildKey(redis.HOMEGATE_USERSLOT_AUTH, v)
			dn, err2 := redisclient.DelZsetLessThen(skey, tnow)
			if err2 != nil && dn > 0 {
				logx.Errorf("res slot maintnce timeout check _HOMEGATE:flexslot key:%s nums:%d", skey, dn)
			} else if err2 != nil {
				logx.Errorf("res slot maintnce timeout check _HOMEGATE:flexslot DelZsetLessThen error:%s", err2.Error())
			}
		}
	}
}

func (p *NetProcessor) doGateFreeSlotSyncMaintnce(frees int32, seq int64, homeid int64, gateid int64) {
	logx.Debugf("recv home syncgateslot notifycation, sequence:%d", seq)

	redisclient := g.GetRedis()

	gkey := fmt.Sprintf(redis.SERVICE_DETAIL_INFO, gateid)
	hgid := fmt.Sprintf("%d#%d", homeid, gateid)
	keys := []string{gkey, redis.HOMEGATE_WATERSLOT, redis.HOMEGATE_FLEXSLOT, redis.GLOBAL_USERSLOT_FREE}

	dat, err := redisclient.Eval(GATEFREESLOT_SYNC, keys, frees, seq, hgid)
	if err != nil {
		logx.Errorf("############# when res service sync gate servie free slots, redis script error err:%s", err.Error())
		//系统错误
		return
	}

	if dat[0] != "0" {
		logx.Debugf("-----------res service sync gate servie free slots error:%s, gate:%d freeslot:%d", dat[0], gateid, frees)
	} else {
		logx.Debugf("-----------res service sync gate servie free slots, gate:%d freeslot:%d totle free:%s", gateid, frees, dat[1])
	}
}
