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

package timerx

import (
	"time"
)

type TimerFunction = func(int64, int, int64) bool

//-----------------------------------------------------------------------
type TimerKey struct {
	step int   //间隔步长，毫秒
	tid  int64 //软定时器id
}

func (t TimerKey) IsValidTimer() bool {
	return t.step > 0 && t.tid > 0
}

func (t TimerKey) IsNone() bool {
	return t.step == 0 || t.tid == 0
}

var TimerKeyNone = TimerKey{}

//-----------------------------------------------------------------------
// 一个time绑定的timer定时器实现
type TimerContainer struct {
	timers map[int]*timerSequence
	//定时最小间隔millisecond
	minTickerStep int
	Time          *time.Ticker
	//计时器运行
	bStart bool
}

// 新建一个定时器对象
func NewTimerContainer(minticker int) (t *TimerContainer) {
	t = new(TimerContainer)
	t.timers = make(map[int]*timerSequence)
	t.minTickerStep = minticker
	t.bStart = false

	return
}

func (c *TimerContainer) Start() {
	// 0.5s ticked
	c.Time = time.NewTicker(time.Millisecond * time.Duration(c.minTickerStep))
}

func (c *TimerContainer) Stop() {
	c.Time.Stop()
}

func (c *TimerContainer) SetRealState(start bool) {
	c.bStart = start
}

func (c *TimerContainer) AddTimer(step int, fun TimerFunction) (tk TimerKey) {
	if step <= 0 {
		tk = TimerKeyNone
		return
	}

	ts, ok := c.timers[step]
	if !ok {
		ts = newTimerSequence(step)
		c.timers[step] = ts
	}

	tid := ts.addTimer(fun)
	tk.step = step
	tk.tid = tid

	return
}

func (c *TimerContainer) DelTimer(tk TimerKey) {
	ts, ok := c.timers[tk.step]
	if !ok {
		tk = TimerKeyNone
		return
	}

	ts.delTimer(tk.tid)
}

func (c *TimerContainer) DelAllTimer() {
	c.timers = make(map[int]*timerSequence)
}

func (c *TimerContainer) Tick(now int64) {
	if !c.bStart {
		return
	}

	for _, v := range c.timers {
		v.tick(now)
	}
}
