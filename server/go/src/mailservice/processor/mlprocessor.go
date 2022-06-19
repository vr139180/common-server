package processor

import (
	"cmslib/server"
	"mailservice/xinf"
	"sync"
)

type MailBoxProcessor struct {
	//处理goroutine
	loopWG      sync.WaitGroup
	vloops      []*mailBoxProcessorLoop
	sysmailLoop *mailBoxProcessorLoop
	vloopNum    int
}

func NewMailBoxProcessor(loopNum int) (p *MailBoxProcessor) {
	p = new(MailBoxProcessor)

	p.vloopNum = 0

	for i := 0; i < loopNum; i++ {
		l := newMailBoxProcessorLoop(p, i)

		p.vloops = append(p.vloops, l)
		p.vloopNum++
	}
	//system mail channel
	p.sysmailLoop = newMailBoxProcessorLoop(p, 0)

	return p
}

func (v *MailBoxProcessor) Start() {
	v.loopWG.Add(v.vloopNum + 1)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}

	go v.sysmailLoop.run()
}

func (v *MailBoxProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}
	{
		v.sysmailLoop.sys <- server.ErrExitApp
		close(v.sysmailLoop.sys)
		close(v.sysmailLoop.ch)
	}

	v.loopWG.Wait()
}

func (p *MailBoxProcessor) PostMailToProcessor(loopIndex int, c xinf.IMailProcessor) {
	p.vloops[loopIndex].ch <- c
}

func (p *MailBoxProcessor) PostToSysMailProcessor(c xinf.IMailProcessor) {
	p.sysmailLoop.ch <- c
}
