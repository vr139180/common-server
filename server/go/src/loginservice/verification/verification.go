package verification

import (
	"cmslib/gnet"
	"cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"loginservice/processor"
	"sync"

	"github.com/emirpasic/gods/lists/arraylist"
)

//验证控制器
type VerifyControl struct {

	//处理goroutine
	loopWG           sync.WaitGroup
	currentLoopIndex int
	vloops           []*verifyLoop
	vloopNum         int

	//maintnce net session
	cond      *sync.Cond
	softTimer *timerx.TimerContainer
	sys       chan interface{}

	session2Wait map[gnet.Conn]*sessionTimeoutCheck
	waitList     *arraylist.List
}

func NewVerifyControl(loopNum int) (vc *VerifyControl) {
	vc = new(VerifyControl)
	vc.currentLoopIndex = 0

	for i := 0; i < loopNum; i++ {
		l := newVerifyLoop(vc)

		vc.vloops = append(vc.vloops, l)
		vc.vloopNum++
	}

	vc.cond = sync.NewCond(&sync.Mutex{})
	vc.softTimer = timerx.NewTimerContainer(1 * 1000)
	vc.sys = make(chan interface{}, 1)
	vc.session2Wait = make(map[gnet.Conn]*sessionTimeoutCheck)
	vc.waitList = arraylist.New()

	vc.registerTimer()

	return
}

func (v *VerifyControl) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
	v.loopWG.Add(1)
	go v.run()
}

func (v *VerifyControl) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].ch <- server.ErrExitApp
		close(v.vloops[i].ch)
	}

	v.sys <- server.ErrExitApp
	close(v.sys)

	v.loopWG.Wait()
}

func (v *VerifyControl) run() {
	defer func() {
		v.softTimer.Stop()
		v.loopWG.Done()
	}()

	v.softTimer.Start()
	v.softTimer.SetRealState(true)

	var err error

	for {
		now := utilc.GetTimestamp()
		select {
		case <-v.softTimer.Time.C:
			v.softTimer.Tick(now)
		case x := <-v.sys:
			switch y := x.(type) {
			case error:
				err = y
			}
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (v *VerifyControl) NewTcpConnection(tcpConn gnet.Conn, t int, uid int64, token string, account string, pwd string) (p *processor.LoginProcessor) {

	//remove from wait list
	v.RemoveNetSession(tcpConn)

	p = processor.NewLoginProcessor(tcpConn)
	//save login information
	p.LoginType = t
	p.LoginUserId = uid
	p.UserToken = token
	p.UserAccount = account
	p.AccountPwd = pwd

	v.vloops[v.currentLoopIndex].ch <- p
	v.currentLoopIndex++
	if v.currentLoopIndex >= v.vloopNum {
		v.currentLoopIndex = 0
	}

	return
}
