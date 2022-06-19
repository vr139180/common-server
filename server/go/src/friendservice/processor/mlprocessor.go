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
