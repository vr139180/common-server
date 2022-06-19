package processor

import (
	"cmslib/gnet"
	"cmslib/gnet/pkg/errors"
	"cmslib/logx"
	"cmslib/server"
	"gamelib/protobuf/gpro"
	"sync"

	"google.golang.org/protobuf/proto"
)

type ProcessorCmdFunction = func(gnet.NetSession, gpro.ERK_PROTYPE, proto.Message)

type NetCmdProcessor struct {
	sess gnet.NetSession
	id   gpro.ERK_PROTYPE
	msg  proto.Message
	fun  ProcessorCmdFunction
}

func NewNetCmdProcessor(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message, f ProcessorCmdFunction) (cmd *NetCmdProcessor) {
	cmd = new(NetCmdProcessor)
	cmd.sess = s
	cmd.id = id
	cmd.msg = m
	cmd.fun = f

	return
}

func (n *NetCmdProcessor) Run() {
	if n.sess != nil && n.msg != nil && n.fun != nil {
		n.fun(n.sess, n.id, n.msg)
	}
}

type NetProcessor struct {
	//处理goroutine
	loopWG           sync.WaitGroup
	currentLoopIndex int
	vloops           []*netProcessorLoop
	vloopNum         int

	msg2Fun map[gpro.ERK_PROTYPE]ProcessorCmdFunction
}

func NewNetProcessor(loopNum int) (p *NetProcessor) {
	p = new(NetProcessor)

	p.vloopNum = 0
	p.currentLoopIndex = 0

	p.registNetCmd()

	for i := 0; i < loopNum; i++ {
		l := newNetProcessorLoop(p)

		p.vloops = append(p.vloops, l)
		p.vloopNum++
	}

	return p
}

func (v *NetProcessor) Start() {
	v.loopWG.Add(v.vloopNum)
	for i := 0; i < v.vloopNum; i++ {
		go v.vloops[i].run()
	}
}

func (v *NetProcessor) Stop() {
	for i := 0; i < v.vloopNum; i++ {
		v.vloops[i].sys <- server.ErrExitApp
		close(v.vloops[i].sys)
		close(v.vloops[i].ch)
	}

	v.loopWG.Wait()
}

func (s *NetProcessor) registMsgMapFun(id gpro.ERK_PROTYPE, fun ProcessorCmdFunction) {
	if s.msg2Fun == nil {
		s.msg2Fun = make(map[gpro.ERK_PROTYPE]ProcessorCmdFunction)
	}

	s.msg2Fun[id] = fun
}

func (s *NetProcessor) getMsgMapFun(id gpro.ERK_PROTYPE) (fun ProcessorCmdFunction, err error) {
	if s.msg2Fun == nil {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	fun, ok := s.msg2Fun[id]
	if !ok {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	return
}

func (p *NetProcessor) postNetCmd(c *NetCmdProcessor) {
	p.vloops[p.currentLoopIndex].ch <- c

	p.currentLoopIndex++
	if p.currentLoopIndex >= p.vloopNum {
		p.currentLoopIndex = 0
	}
}

func (p *NetProcessor) OnNetProcessor(s gnet.NetSession, id int, msg proto.Message) {
	iid := gpro.ERK_PROTYPE(id)
	fun, err := p.getMsgMapFun(iid)
	if err != nil {
		logx.Errorf("---------NetProcessor no process fun for msg:%d", id)
	} else {
		cmd := NewNetCmdProcessor(s, iid, msg, fun)
		p.postNetCmd(cmd)
	}
}
