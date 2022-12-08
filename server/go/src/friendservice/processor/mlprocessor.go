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

package processor

import (
	"cmslib/server"
	"friendservice/xinf"
	"sync"
)

type FriendProcessor struct {
	//处理goroutine
	loopWG   sync.WaitGroup
	vloops   []*friendProcessorLoop
	vloopNum int
}

func NewFriendProcessor(loopNum int) (p *FriendProcessor) {
	p = new(FriendProcessor)

	p.vloopNum = 0

	for i := 0; i < loopNum; i++ {
		l := newfriendProcessorLoop(p, i)

		p.vloops = append(p.vloops, l)
		p.vloopNum++
	}

	return p
}

func (v *FriendProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *FriendProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}

func (p *FriendProcessor) PostFriendToProcessor(loopIndex int, c xinf.IFriendProcessor) {
	p.vloops[loopIndex].ch <- c
}
