package processor

import (
	"chatservice/xinf"
	"cmslib/server"
	"sync"
)

type ChannelProcessor struct {
	//处理goroutine
	loopWG   sync.WaitGroup
	vloops   []*chatProcessorLoop
	vloopNum int
}

func NewChannelProcessor(loopNum int) (p *ChannelProcessor) {
	p = new(ChannelProcessor)

	p.vloopNum = 0

	for i := 0; i < loopNum; i++ {
		l := newChatProcessorLoop(p, i)

		p.vloops = append(p.vloops, l)
		p.vloopNum++
	}

	return p
}

func (v *ChannelProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *ChannelProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}

func (p *ChannelProcessor) PostChatToProcessor(loopIndex int, c xinf.IChatProcessor) {
	p.vloops[loopIndex].ch <- c
}
