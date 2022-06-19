package channel

import (
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

type SingleChannel struct {
	channelBase
}

func newSingleChannel(cid int64, cind int) (pc *SingleChannel) {
	pc = new(SingleChannel)
	pc.InitBase(ChannelTypeSingle_Index, cid, cind)

	return
}

func (cb *SingleChannel) IsPreDefined() bool {
	return false
}

func (cb *SingleChannel) UserSay(msg *gpro.Chat_UserMsgSay) {

	_, uid := ParseUserGate(uint64(msg.Utoken.Giduid))
	item := cb.saveMessage(uid, msg)

	tnow := utilc.GetTimestamp()
	for iu := cb.usersLink.GetHeadElement(); iu != nil; {
		uinfo := iu.(*UserInfo)
		if !uinfo.IsActived(tnow) {
			break
		}

		uinfo.SendChatMsg(item, cb)

		iu = iu.DLGetNextObj()
	}
}
