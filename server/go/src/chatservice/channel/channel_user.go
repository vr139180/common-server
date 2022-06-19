package channel

import (
	"chatservice/g"
	"cmslib/datas"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

type UserInfo struct {
	datas.DoubleLinkBase

	userIid int64
	gateIid int64
	//user token head
	sidUid    int64
	slotToken int64

	//最后更新时间
	lastUpdate int64
}

func NewUserInfo(siduid int64, slottoken int64) (u *UserInfo) {
	u = new(UserInfo)
	u.sidUid = siduid
	u.slotToken = slottoken
	u.gateIid, u.userIid = ParseUserGate(uint64(siduid))

	u.UpdateLasttime()

	return
}

func (u *UserInfo) UpdateLasttime() {
	u.lastUpdate = utilc.GetTimestamp()
}

func (u *UserInfo) IsActived(tnow int64) bool {
	return ((u.lastUpdate + CHANNEL_USER_ACTIVE_STEP) >= tnow)
}

func (u *UserInfo) SendChatMsg(item *gpro.ChatMessageItem, ch IChannel) {

	msg := &gpro.Chat_ChatMsgNtf{}
	msg.Utoken = &gpro.UserToken{Giduid: u.sidUid, Slottoken: u.slotToken}
	msg.Channel = &gpro.ChatChannelInfo{Type: GetCTypeOfChannelType(ch.GetType()), Channeldid: ch.GetChannelId()}
	nitem := proto.Clone(item)
	msg.Msgs = &gpro.ChatMessageItems{}
	msg.Msgs.Msgs = append(msg.Msgs.Msgs, nitem.(*gpro.ChatMessageItem))

	g.SendMsgToRouter(msg)
}

//0b0000000000000000000001111111111111111111111111111111111111111111
const USERGATE_MASK_ZERO_H = uint64(0x7FFFFFFFFFF)

//0b1111111111111111111110000000000000000000000000000000000000000000
const USERGATE_MASK_ZERO_L = uint64(0xFFFFF80000000000)

func ParseUserGate(uidgid uint64) (gateid int64, userid int64) {
	gateid = int64((uidgid & USERGATE_MASK_ZERO_L) >> 43)
	userid = int64((uidgid & USERGATE_MASK_ZERO_H))

	return
}
