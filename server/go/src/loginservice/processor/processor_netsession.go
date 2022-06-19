package processor

import (
	"gamelib/protobuf/gpro"
	"loginservice/g"
	"sync/atomic"

	"google.golang.org/protobuf/proto"
)

//发送资源分配请求
func (l *LoginProcessor) AskLoginAsset() *gpro.Res_UserSlotReq {
	req := &gpro.Res_UserSlotReq{}
	req.Sequence = l.SeqIid
	req.UserIid = l.UserIid

	return req
}

//--------------------gnet.NetSession interface-----------------------
func (l *LoginProcessor) OnRecvMessage(id int, pro proto.Message) {
	g.RegistProcessNetCmd(l, gpro.ERK_PROTYPE(id), pro)
}

func (l *LoginProcessor) OnClose() {
	l.BaseNetSession.OnClose()

	atomic.StoreInt32(&l.isNetClosed, 1)
}
