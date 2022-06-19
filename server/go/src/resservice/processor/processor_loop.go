package processor

import "cmslib/server"

const netProcessorLoopChanCaption = 1024

type netProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	parent *NetProcessor
}

func newNetProcessorLoop(p *NetProcessor) (l *netProcessorLoop) {
	l = new(netProcessorLoop)
	l.parent = p
	l.ch = make(chan interface{}, netProcessorLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *netProcessorLoop) run() {
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
		case i := <-l.ch:
			switch v := i.(type) {
			case *NetCmdProcessor:
				v.Run()
			}
		}

		if err == server.ErrExitApp {
			break
		}
	}
}
