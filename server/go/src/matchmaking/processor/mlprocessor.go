package processor

import (
	"cmslib/server"
	"sync"
)

type MatchMakingProcessor struct {
	//处理goroutine
	loopWG   sync.WaitGroup
	vloops   []*matchmakingProcessorLoop
	vloopNum int
}

func NewMatchMakingdProcessor(loopNum int) (p *MatchMakingProcessor) {
	p = new(MatchMakingProcessor)

	p.vloopNum = 0

	for i := 0; i < loopNum; i++ {
		l := newMatchMakingProcessorLoop(p, i)

		p.vloops = append(p.vloops, l)
		p.vloopNum++
	}

	return p
}

func (v *MatchMakingProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *MatchMakingProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}
