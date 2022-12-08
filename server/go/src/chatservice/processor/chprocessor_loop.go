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
	"chatservice/g"
	"chatservice/xinf"
	"cmslib/server"
	"cmslib/utilc"
	"time"
)

const cahtLoopChanCaption = 1024

type chatProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	maintanceTicker *time.Ticker

	channelIndex int
	parent       *ChannelProcessor
}

func newChatProcessorLoop(p *ChannelProcessor, cindex int) (l *chatProcessorLoop) {
	l = new(chatProcessorLoop)
	l.parent = p
	l.channelIndex = cindex
	l.ch = make(chan interface{}, cahtLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *chatProcessorLoop) run() {

	defer func() {
		l.maintanceTicker.Stop()
		l.parent.loopWG.Done()
	}()

	var err error
	l.timingMaintance()

	for {
		select {
		case ii := <-l.sys:
			switch v := ii.(type) {
			case error:
				err = v
			}
		case i := <-l.ch:
			switch v := i.(type) {
			case xinf.IChatProcessor:
				v.ProcessChat()
			}
		case <-l.maintanceTicker.C:
			l.maintanceChannelTicker()
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (l *chatProcessorLoop) maintanceChannelTicker() {
	l.timingMaintance()

	g.DoChannelMaintance(l.channelIndex)
}

func (l *chatProcessorLoop) timingMaintance() {

	tn := time.Now()

	nday := time.Date(tn.Year(), tn.Month(), tn.Day(), 23, 59, 59, 999, tn.Location())
	hs := 1 + utilc.CMSRandMax(3)
	nday = nday.Add(time.Hour * time.Duration(hs))
	ms := 5 + utilc.CMSRandMax(30)
	nday = nday.Add(time.Minute * time.Duration(ms))

	delay := nday.Unix() - tn.Unix()

	if l.maintanceTicker == nil {
		l.maintanceTicker = time.NewTicker(time.Second * time.Duration(delay))
	} else {
		l.maintanceTicker.Reset(time.Second * time.Duration(delay))
	}
}
