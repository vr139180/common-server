package processor

import (
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/utilc"
	"fmt"
	"gamelib/protobuf/gpro"
	"gamelib/redis"
	"resservice/g"
	"strconv"

	"google.golang.org/protobuf/proto"
)

func (p *NetProcessor) registNetCmd() {
	p.registMsgMapFun(gpro.ERK_PROTYPE_RES_USERSLOT_REQ, p.OnResUserSlotReq)
	p.registMsgMapFun(gpro.ERK_PROTYPE_RES_PROXYUSERSLOT_ACK, p.OnResProxyUserSlotAck)
	p.registMsgMapFun(gpro.ERK_PROTYPE_RES_SYNCGATESLOT_NTF, p.OnResSyncGateSlotNtf)
}

//from login service slot request
func (p *NetProcessor) OnResUserSlotReq(ns gnet.NetSession, id gpro.ERK_PROTYPE, msg proto.Message) {
	req := msg.(*gpro.Res_UserSlotReq)
	ts := utilc.GetTimestamp()

	proxyToken := utilc.UuidGen()

	logx.Debugf("recv Res_UserSlotReq from login service userid:%d token:%s", req.GetUserIid(), proxyToken)

	redisclient := g.GetRedis()
	keys := []string{redis.HOMEGATE_WATERSLOT, redis.HOMEGATE_FLEXSLOT,
		redis.HOMEGATE_USERSLOT_AUTH, redis.GLOBAL_USERSLOT_FREE}

	dat, err := redisclient.Eval(USERSLOT_REQUEST_SCRIPT, keys, proxyToken, ts)
	if err != nil {
		logx.Errorf("when res service assign gate service slot, redis script run failed err:%s", err.Error())

		ack := &gpro.Res_UserSlotAck{}
		ack.Result = 2
		ack.Lsno = req.GetLsno()
		ack.Sequence = req.GetSequence()
		ack.UserIid = req.GetUserIid()

		ns.SendMessage(ack)

		return
	}

	if dat[0] != "0" {
		logx.Debugf("res service not free gate slot to assign. redis script ret:%s", dat[0])

		ack := &gpro.Res_UserSlotAck{}
		ack.Result = 1
		ack.Lsno = req.GetLsno()
		ack.Sequence = req.GetSequence()
		ack.UserIid = req.GetUserIid()

		ns.SendMessage(ack)

		return
	}

	hgid := utilc.SpliteStrToInt64(dat[1], "#")
	ty, _ := strconv.Atoi(dat[2])

	ask := &gpro.Res_ProxyUserSlotReq{}
	ask.Proxytoken = proxyToken
	ask.Lsno = req.Lsno
	ask.Sequence = req.Sequence
	ask.UserIid = req.GetUserIid()
	ask.Homeiid = hgid[0]
	ask.Gateiid = hgid[1]
	ask.Hgtype = int32(ty)

	g.SendMessageToHome(ask.Homeiid, ask)

	logx.Debugf("res service send proxyuser slot request to home service:[%d,%d] userid:%d token:%s", hgid[0], hgid[1], ask.UserIid, ask.Proxytoken)
}

//from home service proxy slot ack
func (p *NetProcessor) OnResProxyUserSlotAck(ns gnet.NetSession, id gpro.ERK_PROTYPE, msg proto.Message) {
	ack := msg.(*gpro.Res_ProxyUserSlotAck)

	logx.Debugf("recv Res_ProxyUserSlotAck:%d from home service", ack.GetResult())

	resask := &gpro.Res_UserSlotAck{}
	resask.Lsno = ack.Lsno
	resask.Sequence = ack.Sequence
	resask.UserIid = ack.UserIid
	if ack.GetResult() == 0 {
		resask.Slot = proto.Int32(ack.GetSlot())
		resask.Proxyip = proto.String(ack.GetProxyip())
		resask.Proxyport = proto.Int32(ack.GetProxyport())
		resask.Slottoken = proto.Int64(ack.GetSlottoken())
	}

	if ack.GetResult() == 0 {
		resask.Result = 0
	} else {
		resask.Result = 1
	}

	//成功或者 slot资源不足失败
	if ack.GetResult() == 0 || ack.GetResult() == 3 {
		redisclient := g.GetRedis()

		hgid := fmt.Sprintf("%d#%d", ack.Homeiid, ack.Gateiid)
		authkey := redisclient.BuildKey(redis.HOMEGATE_USERSLOT_AUTH, hgid)
		keys := []string{authkey}

		dat, err := redisclient.Eval(USERSLOT_CONFIRM_SCRIPT, keys, ack.GetProxytoken())
		if err != nil {
			logx.Errorf("############# when res service confirm OnResProxyUserSlotAck, redis script error err:%s", err.Error())
			//系统错误
			resask.Result = 2
		}

		if dat[0] != "0" {
			logx.Debugf("-----------when res service confirm OnResProxyUserSlotAck, token:%s not exist in the:%s", ack.GetProxytoken(), authkey)
			//token不存在，标记为失败
			if resask.Result == 0 {
				resask.Result = 1
			}
		}
	}

	g.SendMessageToLogin(ack.Lsno, resask)
}

func (p *NetProcessor) OnResSyncGateSlotNtf(ns gnet.NetSession, id gpro.ERK_PROTYPE, msg proto.Message) {
	ntf := msg.(*gpro.Res_SyncGateSlotNtf)

	p.doGateFreeSlotSyncMaintnce(ntf.GetFreeslots(), ntf.GetSyncsequence(), ntf.GetHomeiid(), ntf.GetGateiid())
}
