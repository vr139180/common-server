package channel

import (
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

type PreAllocChannel struct {
	channelBase
}

func newPreAllocChannel(t ChannelType, cid int64, cind int) (pc *PreAllocChannel) {
	pc = new(PreAllocChannel)
	pc.InitBase(t, cid, cind)

	return
}

func (cb *PreAllocChannel) UserSay(msg *gpro.Chat_UserMsgSay) {
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
