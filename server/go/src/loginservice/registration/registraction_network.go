package registration

import (
	"cmslib/gnet"
	"cmslib/logx"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

func (r *RegistControl) registNetCmdProcess() {
	r.registMsgMapFun(gpro.ERK_PROTYPE_RES_USERSLOT_ACK, r.OnResUserSlotAck)
}

//from res service
func (r *RegistControl) OnResUserSlotAck(ns gnet.NetSession, id gpro.ERK_PROTYPE, msg proto.Message) {
	ack := msg.(*gpro.Res_UserSlotAck)
	logx.Debugf("recv Res_UserSlotAck from res service result:%d seq:%d uid:%d lsno:%d", ack.Result, ack.Sequence, ack.UserIid, ack.Lsno)
	if ack.Result == 0 {
		logx.Debugf("Res_UserSlotAck result slot:%d utoken:%d ip:%s port:%d", ack.GetSlot(), ack.GetSlottoken(), ack.GetProxyip(), ack.GetProxyport())
	}

	user := r.removeAssignWait(ack.GetSequence())
	if user == nil {
		logx.Debugf("RegistControl->OnResUserSlotAck not found sequence:%d", ack.GetSequence())

		user.Close()
		return
	}

	if user.UserIid != ack.GetUserIid() {
		logx.Debugf("RegistControl->OnResUserSlotAck sequence:%d is not same user:%d[r:%d]", ack.GetSequence(), ack.GetUserIid(), user.UserIid)

		user.Close()
		return
	}

	if !user.IsValidProcessor() {
		logx.Debugf("RegistControl->OnResUserSlotAck(*processor.LoginProcessor) user.IsValidProcessor == false")
		return
	}

	if ack.GetResult() == 0 {
		xret := &gpro.User_LoginAck{}
		xret.Type = int32(user.LoginType)
		xret.Result = 0

		xret.UserIid = proto.Int64(user.UserIid)
		xret.Proxytoken = proto.Int64(ack.GetSlottoken())
		xret.Slot = proto.Int32(ack.GetSlot())
		xret.Proxyip = proto.String(ack.GetProxyip())
		xret.Proxyport = proto.Int32(ack.GetProxyport())
		xret.Logintoken = proto.String(user.LoginToken)

		user.SendMessage(xret)

		//r.dealDisconnect.AddTailElement(user)
		user.Close()
	} else {
		//wait
		r.registWaiting.AddHeadElement(user)
	}
}
