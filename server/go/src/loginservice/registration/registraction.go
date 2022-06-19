package registration

import (
	"cmslib/datas"
	"cmslib/server"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"loginservice/processor"
	"sync"
	"time"
)

type RegistControl struct {
	//排队等待链表
	registWaiting *datas.DoubleLinkSafe
	//分配返回等待链表
	assignWaiting *datas.DoubleLinkSafe
	//延迟断开列表
	dealDisconnect *datas.DoubleLinkSafe

	//和seqiid关联的 IDoubleLink对象
	cond                 *sync.Cond
	seqUserAssignWaiting map[int64]datas.IDoubleLink
	seqIidGenerator      int64

	//netcmd fun map
	msg2Fun map[gpro.ERK_PROTYPE]netCmdFunction

	//处理registwaiting的池
	loopPools []*registLoop

	//仅接受返回的消息处理
	netack chan interface{}
	sysch  chan interface{}

	loopWG sync.WaitGroup
}

func NewRegistControl(loopNum int) (r *RegistControl) {
	r = new(RegistControl)
	r.registWaiting = datas.NewDoubleLinkSafe()
	r.assignWaiting = datas.NewDoubleLinkSafe()
	r.dealDisconnect = datas.NewDoubleLinkSafe()
	r.seqUserAssignWaiting = make(map[int64]datas.IDoubleLink)
	r.seqIidGenerator = 1
	r.cond = sync.NewCond(&sync.Mutex{})

	r.netack = make(chan interface{}, 1024)
	r.sysch = make(chan interface{}, 1)

	r.registNetCmdProcess()

	for i := 0; i < loopNum; i++ {
		l := newRegistLoop(r)
		r.loopPools = append(r.loopPools, l)
	}

	return
}

func (r *RegistControl) Start() {
	r.loopWG.Add(1)
	r.loopWG.Add(len(r.loopPools))
	go r.run()

	for i := 0; i < len(r.loopPools); i++ {
		go r.loopPools[i].run()
	}
}

func (r *RegistControl) Stop() {
	r.registWaiting.ClearAllElement()
	r.assignWaiting.ClearAllElement()

	for i := 0; i < len(r.loopPools); i++ {
		r.loopPools[i].sysch <- server.ErrExitApp
		close(r.loopPools[i].sysch)
	}

	close(r.netack)
	r.sysch <- server.ErrExitApp
	close(r.sysch)

	r.loopWG.Wait()
}

func (r *RegistControl) run() {
	var err error
	dotimer := time.NewTicker(time.Second * 5)

	defer func() {
		dotimer.Stop()
		r.loopWG.Done()
	}()

	for {
		select {
		case i := <-r.sysch:
			switch v := i.(type) {
			case error:
				err = v
			}
		case a := <-r.netack:
			switch v := a.(type) {
			case *netCmdProcessor:
				v.run()
			}
		case <-dotimer.C:
			r.assignWaitTimeoutCheck()
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (r *RegistControl) addAssignWait(p *processor.LoginProcessor) {
	defer r.cond.L.Unlock()
	r.cond.L.Lock()

	p.AssignTime = utilc.GetTimestamp()

	r.assignWaiting.AddTailElement(p)
	r.seqUserAssignWaiting[p.SeqIid] = p
}

func (r *RegistControl) removeAssignWait(seqid int64) *processor.LoginProcessor {
	defer r.cond.L.Unlock()
	r.cond.L.Lock()

	d, ok := r.seqUserAssignWaiting[seqid]
	if !ok {
		return nil
	}

	delete(r.seqUserAssignWaiting, seqid)
	r.assignWaiting.DelElement(d)

	return d.(*processor.LoginProcessor)
}

func (r *RegistControl) assignWaitTimeoutCheck() {
	tnow := utilc.GetTimestamp()

	defer r.cond.L.Unlock()
	r.cond.L.Lock()

	for {
		h := r.assignWaiting.PopupHeadElement()
		if h == nil {
			return
		}

		d := h.(*processor.LoginProcessor)
		if d.AssignTime+5000 > tnow {
			r.assignWaiting.AddHeadElement(h)
			return
		}

		delete(r.seqUserAssignWaiting, d.SeqIid)

		//放回等待分配队列
		r.registWaiting.AddHeadElement(h)
	}

}
