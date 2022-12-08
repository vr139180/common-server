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

package dbprocessor

import (
	"cmslib/server"
	"mailservice/xinf"
	"sync"
)

const dbLoopChanCaption = 1024

type DBMailProcessor struct {
	//处理goroutine
	loopWG   sync.WaitGroup
	vloops   []*dbProcessorLoop
	vloopNum int

	curLoopIndex int
}

func NewDBMailProcessor(lnum int) (dm *DBMailProcessor) {
	dm = new(DBMailProcessor)

	dm.vloopNum = 0
	dm.curLoopIndex = 0

	for i := 0; i < lnum; i++ {
		l := newDBProcessorLoop(i, dm)

		dm.vloops = append(dm.vloops, l)
		dm.vloopNum++
	}

	return
}

func (v *DBMailProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *DBMailProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}

func (v *DBMailProcessor) PostDBProcessor(cmd xinf.IDBProcessor) {
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
	parent  *DBMailProcessor
}

func newDBProcessorLoop(ind int, p *DBMailProcessor) (l *dbProcessorLoop) {
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
