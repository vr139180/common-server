package channel

import (
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

type CustomChannel struct {
	channelBase
}

func newCustomChannel(cid int64, cind int) (pc *CustomChannel) {
	pc = new(CustomChannel)
	pc.InitBase(ChannelTypeCustom_Index, cid, cind)

	return
}

func (cb *CustomChannel) IsPreDefined() bool {
	return false
}

func (cb *CustomChannel) UserSay(msg *gpro.Chat_UserMsgSay) {
	say := cb.getUserOfMsg(msg)
	if say == nil {
		return
	}

	item := cb.saveMessage(say.userIid, msg)

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
