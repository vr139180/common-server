package processor

import (
	"cmslib/server"
	"cmslib/utilc"
	"mailservice/g"
	"mailservice/xinf"
	"time"
)

const mailLoopChanCaption = 1024

type mailBoxProcessorLoop struct {
	//ch recv event
	ch chan interface{}
	//sys event
	sys chan interface{}

	maintanceTicker *time.Ticker

	mbIndex int
	parent  *MailBoxProcessor
}

func newMailBoxProcessorLoop(p *MailBoxProcessor, cindex int) (l *mailBoxProcessorLoop) {
	l = new(mailBoxProcessorLoop)
	l.parent = p
	l.mbIndex = cindex
	l.ch = make(chan interface{}, mailLoopChanCaption)
	l.sys = make(chan interface{}, 1)

	return
}

func (l *mailBoxProcessorLoop) run() {

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
			case xinf.IMailProcessor:
				v.ProcessMail()
			}
		case <-l.maintanceTicker.C:
			l.maintanceMaiBoxTicker()
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (l *mailBoxProcessorLoop) maintanceMaiBoxTicker() {
	l.timingMaintance()

	g.DoMailBoxMaintance(l.mbIndex)
}

func (l *mailBoxProcessorLoop) timingMaintance() {

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
