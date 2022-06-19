package dbserialize

import (
	"cmslib/server"
	"sync"
	"time"
)

type DBSerializePool struct {

	//处理registwaiting的池
	loopPools []*serializeLoop
	//当前pool
	curCmdIndex int
	loopWG      sync.WaitGroup

	sysch chan interface{}
}

func NewDBSerializePool(loopNum int) (r *DBSerializePool) {
	r = new(DBSerializePool)

	r.sysch = make(chan interface{}, 1)

	for i := 0; i < loopNum; i++ {
		l := newSerializeLoop(r)
		r.loopPools = append(r.loopPools, l)
	}
	r.curCmdIndex = 0

	return
}

func (r *DBSerializePool) Start() {
	r.loopWG.Add(1)
	r.loopWG.Add(len(r.loopPools))
	go r.run()

	for i := 0; i < len(r.loopPools); i++ {
		go r.loopPools[i].run()
	}
}

func (r *DBSerializePool) Stop() {

	for i := 0; i < len(r.loopPools); i++ {
		r.loopPools[i].sysch <- server.ErrExitApp
		close(r.loopPools[i].sysch)
	}

	r.sysch <- server.ErrExitApp
	close(r.sysch)

	r.loopWG.Wait()
}

func (r *DBSerializePool) run() {
	var err error
	dotimer := time.NewTicker(time.Second * 5)

	defer func() {
		dotimer.Stop()
		r.loopWG.Done()
	}()

	for {
		select {
		case i := <-r.sysch:
			switch v := i.(type) {
			case error:
				err = v
			}

		case <-dotimer.C:
			r.timer_roleserialize_queue(500)
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (r *DBSerializePool) dbCmdPool() (l *serializeLoop) {
	l = r.loopPools[r.curCmdIndex]

	r.curCmdIndex++
	if r.curCmdIndex >= len(r.loopPools) {
		r.curCmdIndex = 0
	}

	return
}
