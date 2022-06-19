package registration

import (
	"cmslib/server"
	"gamelib/redis"
	"loginservice/g"
	"loginservice/processor"
	"strconv"
	"time"
)

type registLoop struct {
	//100 mill的定时器
	evTick *time.Ticker
	//单个的系统消息处理
	sysch chan interface{}

	parent *RegistControl
}

func newRegistLoop(p *RegistControl) (l *registLoop) {
	l = new(registLoop)
	l.sysch = make(chan interface{}, 1)
	l.parent = p

	return
}

func (l *registLoop) run() {

	defer func() {
		l.parent.loopWG.Done()
		if l.evTick != nil {
			l.evTick.Stop()
		}
	}()

	l.evTick = time.NewTicker(time.Millisecond * 100)

	var err error
	for {
		select {
		case i := <-l.sysch:
			switch v := i.(type) {
			case error:
				err = v
			}

		case <-l.evTick.C:
			l.doRegist()
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (l *registLoop) doRegist() {
	loopnum := 5

	redisclient := g.GetRedis()
	fs, err := redisclient.Get(redis.GLOBAL_USERSLOT_FREE)
	if err == nil {
		fsn, err2 := strconv.Atoi(fs)
		if err2 == nil {
			if fsn <= 0 {
				return
			} else {
				if loopnum > fsn {
					loopnum = fsn
				}
			}
		} else {
			loopnum = 1
		}
	} else {
		loopnum = 1
	}

	for i := 0; i < loopnum; i++ {
		v := l.parent.registWaiting.PopupHeadElement()
		if v == nil {
			break
		}

		p := v.(*processor.LoginProcessor)
		if !p.IsValidProcessor() {
			continue
		}

		g.AskResUserSlot(p)

		//添加关联
		l.parent.addAssignWait(p)
	}
}
