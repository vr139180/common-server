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
