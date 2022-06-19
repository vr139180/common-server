package dbprocessor

import (
	"cmslib/server"
	"friendservice/xinf"
	"sync"
)

const dbLoopChanCaption = 1024

type DBFriendProcessor struct {
	//处理goroutine
	loopWG   sync.WaitGroup
	vloops   []*dbProcessorLoop
	vloopNum int

	curLoopIndex int
}

func NewDBFriendProcessor(lnum int) (dm *DBFriendProcessor) {
	dm = new(DBFriendProcessor)

	dm.vloopNum = 0
	dm.curLoopIndex = 0

	for i := 0; i < lnum; i++ {
		l := newDBProcessorLoop(i, dm)

		dm.vloops = append(dm.vloops, l)
		dm.vloopNum++
	}

	return
}

func (v *DBFriendProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *DBFriendProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}

func (v *DBFriendProcessor) PostDBProcessor(cmd xinf.IDBProcessor) {
	v.vloops[v.curLoopIndex].ch <- cmd

	v.curLoopIndex++
	if v.curLoopIndex >= v.vloopNum {
		v.curLoopIndex = 0
	}
}

//-----------------------------------------------------------------------------------
type dbProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	mbIndex int
	parent  *DBFriendProcessor
}

func newDBProcessorLoop(ind int, p *DBFriendProcessor) (l *dbProcessorLoop) {
	l = new(dbProcessorLoop)

	l.parent = p
	l.mbIndex = ind
	l.ch = make(chan interface{}, dbLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *dbProcessorLoop) run() {

	defer func() {
		l.parent.loopWG.Done()
	}()

	var err error

	for {
		select {
		case ii := <-l.sys:
			switch v := ii.(type) {
			case error:
				err = v
			}
		case xv := <-l.ch:
			switch v := xv.(type) {
			case xinf.IDBProcessor:
				v.RunInDBProcessor()
			}
		}

		if err == server.ErrExitApp {
			break
		}
	}
}
