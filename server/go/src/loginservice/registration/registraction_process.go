package registration

import (
	"cmslib/gnet"
	"cmslib/gnet/pkg/errors"
	"cmslib/logx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"loginservice/processor"
	"sync/atomic"

	"google.golang.org/protobuf/proto"
)

type netCmdFunction = func(gnet.NetSession, gpro.ERK_PROTYPE, proto.Message)

type netCmdProcessor struct {
	sess gnet.NetSession
	id   gpro.ERK_PROTYPE
	msg  proto.Message
	fun  netCmdFunction
}

func newNetCmdProcessor(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message, f netCmdFunction) (cmd *netCmdProcessor) {
	cmd = new(netCmdProcessor)
	cmd.sess = s
	cmd.id = id
	cmd.msg = m
	cmd.fun = f

	return
}

func (n *netCmdProcessor) run() {
	if n.sess != nil && n.msg != nil && n.fun != nil {
		n.fun(n.sess, n.id, n.msg)
	}
}

func (r *RegistControl) RegistWaitProcess(p *processor.LoginProcessor) {
	nid := atomic.AddInt64(&r.seqIidGenerator, 1)
	p.SeqIid = nid
	p.RegistRequestTime = utilc.GetTimestamp()

	r.registWaiting.AddTailElement(p)
}

func (r *RegistControl) registMsgMapFun(id gpro.ERK_PROTYPE, fun netCmdFunction) {
	if r.msg2Fun == nil {
		r.msg2Fun = make(map[gpro.ERK_PROTYPE]netCmdFunction)
	}

	r.msg2Fun[id] = fun
}

func (r *RegistControl) getMsgMapFun(id gpro.ERK_PROTYPE) (fun netCmdFunction, err error) {
	if r.msg2Fun == nil {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	fun, ok := r.msg2Fun[id]
	if !ok {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	return
}

func (r *RegistControl) ProcessNetCmd(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message) {
	fun, err := r.getMsgMapFun(id)
	if err != nil {
		logx.Errorf("RegistControl->ProcessNetCmd not found process of msg:%d", int(id))
	}

	cmd := newNetCmdProcessor(s, id, m, fun)
	r.netack <- cmd
}
