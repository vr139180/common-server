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
