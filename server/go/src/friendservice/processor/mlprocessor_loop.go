package processor

import (
	"cmslib/server"
	"cmslib/utilc"
	"friendservice/g"
	"friendservice/xinf"
	"time"
)

const mailLoopChanCaption = 1024

type friendProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	maintanceTicker *time.Ticker

	mbIndex int
	parent  *FriendProcessor
}

func newfriendProcessorLoop(p *FriendProcessor, cindex int) (l *friendProcessorLoop) {
	l = new(friendProcessorLoop)
	l.parent = p
	l.mbIndex = cindex
	l.ch = make(chan interface{}, mailLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *friendProcessorLoop) run() {

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
			case xinf.IFriendProcessor:
				v.ProcessFriend()
			}
		case <-l.maintanceTicker.C:
			l.maintanceFriendsTicker()
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (l *friendProcessorLoop) maintanceFriendsTicker() {
	l.timingMaintance()

	g.DoFriendsMaintance(l.mbIndex)
}

func (l *friendProcessorLoop) timingMaintance() {

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
