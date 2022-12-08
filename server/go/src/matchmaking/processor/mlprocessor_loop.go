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
)

const mailLoopChanCaption = 1024

type matchmakingProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	mbIndex int
	parent  *MatchMakingProcessor
}

func newMatchMakingProcessorLoop(p *MatchMakingProcessor, cindex int) (l *matchmakingProcessorLoop) {
	l = new(matchmakingProcessorLoop)
	l.parent = p
	l.mbIndex = cindex
	l.ch = make(chan interface{}, mailLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *matchmakingProcessorLoop) run() {

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
		}

		if err == server.ErrExitApp {
			break
		}
	}
}
