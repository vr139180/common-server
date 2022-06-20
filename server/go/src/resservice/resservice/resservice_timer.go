package resservice

import (
	"gamelib/dtask"
	"gamelib/redis"
	"resservice/g"
)

func (l *ResService) RegisterTimer() {
	l.softTimer.AddTimer(3*1000, l.OnUserSlotAuthsMaintnce)
}

func (l *ResService) OnUserSlotAuthsMaintnce(int64, int, int64) (finish bool) {
	//loop timers
	finish = false

	//logic
	//logx.Debugf("ResService User Slot auth maintnce do")
	redisclient := g.GetRedis()

	//4 second
	dt, own, err := dtask.BuildDTaskRedis(redis.GLOBAL_DTASK_RES_SVRMAINTNCE, l.GetMyNode().Iid, 4, redisclient)
	if err != nil {
		return
	}

	if !own {
		// end this timer
		finish = true

		timer := dt.NextTaskTimer(3)
		if timer > 0 {
			l.softTimer.AddTimer(int(timer), l.OnUserSlotAuthsMaintnce)
		}

		return
	}

	//do maintnce
	l.netProcessor.DoUserSlotAuthMaintnce()

	return
}
