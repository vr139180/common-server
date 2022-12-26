// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package channel

import (
	"cmslib/datas"
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
)

//channel保留的最大历史记录
const CHANNLE_MAX_MESSAGE_NUM = 50

//channel 检测是否移动的时间间隔 15s
const CHANNEL_MOVE_CHECK_STEP = int64(15 * 1000)

//channel user活跃度检测间隔
//超过45s的用户认为是离线用户
const CHANNEL_USER_ACTIVE_STEP = int64(45 * 1000)

//channel offuser维护间隔
//user total > 1000 //15分钟
const CHANNEL_OFFUSER_USERMAX = 1000
const CHANNEL_OFFUSER_MAINSTEP = int64(15 * 60 * 1000)

type IChannel interface {
	GetType() ChannelType
	GetLoopIndex() int
	GetChannelId() int64
	//是否预定义的频道,default is true
	IsPreDefined() bool
	//是否超时无激活
	IsActiveTimeout(tnow int64) bool
	//为了减少移动的开销，一定间隔时间之后会进行channel的位置调整
	NeedMoveChannel(tnow int64) bool

	UserActive(*UserInfo, int64)
	UserSay(*protocolx.NetProtocol)

	//定时维护离线用户情况
	//用户数超过1000 需要定时维护，维护间隔15分钟
	MaintanceOfflineUsers(tnow int64, force bool)

	ChannelReset()
}

//----------------------------------implement IChannel interface-------------------
type channelBase struct {
	datas.DoubleLinkBase

	//在线用户
	users map[int64]*UserInfo
	//根据用户的actvie活动进行排列的双向链表
	//方便进行协议转发和活跃度检测
	usersLink *datas.DoubleLinkUnSafe
	//上次调整位置时间
	lastUpdate int64
	//上次维护时间
	lastMaintance int64

	//历史信息
	messages gpro.ChatMessageItems

	//base info
	ctype     ChannelType
	channelId int64
	//分配到的channel goroutine index
	loopIndex int

	//IDoubleLink members
	preObj  datas.IDoubleLink
	nextObj datas.IDoubleLink
}

//-----------------------------------IDoubleLink implement-------------------------------
func (u *channelBase) DLGetPreObj() (n datas.IDoubleLink) {
	n = u.preObj
	return
}

func (u *channelBase) DLSetPreObj(n datas.IDoubleLink) {
	u.preObj = n
}

func (u *channelBase) DLGetNextObj() (n datas.IDoubleLink) {
	n = u.nextObj
	return
}

func (u *channelBase) DLSetNextObj(n datas.IDoubleLink) {
	u.nextObj = n
}

//-----------------------------------IDoubleLink implement-------------------------------

func (cb *channelBase) InitBase(t ChannelType, cid int64, cind int) {
	cb.ctype = t
	cb.channelId = cid
	cb.loopIndex = cind
	cb.users = make(map[int64]*UserInfo)
	cb.usersLink = datas.NewDoubleLinkUnSafe()
	cb.lastUpdate = utilc.GetTimestamp()
	cb.lastMaintance = 0
}

func (cb *channelBase) GetType() ChannelType {
	return cb.ctype
}

func (cb *channelBase) GetLoopIndex() int {
	return cb.loopIndex
}

func (cb *channelBase) GetChannelId() int64 {
	return cb.channelId
}

func (cb *channelBase) IsPreDefined() bool {
	return true
}

func (cb *channelBase) GetUserByIid(uid int64) (u *UserInfo) {
	u, ok := cb.users[uid]

	if !ok {
		u = nil
		return
	}

	return
}

func (cb *channelBase) UserActive(u *UserInfo, tnow int64) {
	ou := cb.GetUserByIid(u.GetRoleIid())
	if ou != nil {
		cb.usersLink.DelElement(ou)
	}

	cb.usersLink.AddHeadElement(u)
	cb.users[u.GetRoleIid()] = u

	//激活处理
	cb.MaintanceOfflineUsers(tnow, false)
}

func (cb *channelBase) UserSay(msg *gpro.Chat_UserMsgSay) {

}

func (cb *channelBase) getUserOfMsg(pro *protocolx.NetProtocol) (u *UserInfo) {
	rid := pro.GetRoleIid()
	u = cb.GetUserByIid(rid)
	if u == nil {
		return
	}

	if u.token != pro.Head.Token {
		u = nil
	}

	return
}

func (cb *channelBase) saveMessage(roleiid int64, msg *gpro.Chat_UserMsgSay) *gpro.ChatMessageItem {

	item := &gpro.ChatMessageItem{}

	item.SendUserIid = roleiid
	item.SendNickname = msg.GetNickname()
	item.SendTime = utilc.GetTimeSecond()
	item.MsgContent = msg.GetMsgContent()
	cb.messages.Msgs = append(cb.messages.Msgs, item)
	if len(cb.messages.Msgs) > CHANNLE_MAX_MESSAGE_NUM {
		cb.messages.Msgs = cb.messages.Msgs[1:]
	}

	return item
}

func (cb *channelBase) IsActiveTimeout(tnow int64) bool {
	return (cb.lastUpdate <= tnow)
}

func (cb *channelBase) NeedMoveChannel(tnow int64) bool {
	if (cb.lastUpdate + CHANNEL_MOVE_CHECK_STEP) < tnow {
		cb.lastUpdate = tnow
		return true
	} else {
		return false
	}
}

func (cb *channelBase) ChannelReset() {
	cb.users = make(map[int64]*UserInfo)
	cb.usersLink.ClearAllElement()
	cb.messages.Reset()
}

func (cb *channelBase) MaintanceOfflineUsers(tnow int64, force bool) {
	if !force {
		if len(cb.users) < CHANNEL_OFFUSER_USERMAX {
			return
		}
		if cb.lastMaintance+CHANNEL_OFFUSER_MAINSTEP > tnow {
			return
		}
	}

	cb.lastMaintance = tnow

	//维护user
	//从队列的尾部开始回溯检测
	for cuser := cb.usersLink.GetTailElement(); cuser != nil; {
		uinfo := cuser.(*UserInfo)
		if uinfo.IsActived(tnow) {
			break
		}

		//remove user
		delete(cb.users, uinfo.GetRoleIid())
		cb.usersLink.DelElement(cuser)

		cuser = cb.usersLink.GetTailElement()
	}

}

//-----------------------------------------------------------------
func NewOneChannel(t ChannelType, cid int64, cindex int) (c IChannel) {
	c = nil
	if t == ChannelTypeGlobal_Indx || t == ChannelTypeUnion_Index || t == ChannelTypeChannel_Index {
		c = newPreAllocChannel(t, cid, cindex)
	} else if t == ChannelTypeSingle_Index {
		c = newSingleChannel(cid, cindex)
	} else if t == ChannelTypeCustom_Index {
		c = newCustomChannel(cid, cindex)
	}

	return
}
