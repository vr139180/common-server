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
	"chatservice/g"
	"cmslib/datas"
	"cmslib/protocolx"
	"cmslib/utilc"
	"gamelib/protobuf/gpro"
	"gamelib/service"

	"google.golang.org/protobuf/proto"
)

type UserInfo struct {
	datas.DoubleLinkBase

	roleiid int64
	token   protocolx.UserToken

	//最后更新时间
	lastUpdate int64

	//IDoubleLink members
	preObj  datas.IDoubleLink
	nextObj datas.IDoubleLink
}

//-----------------------------------IDoubleLink implement-------------------------------
func (u *UserInfo) DLGetPreObj() (n datas.IDoubleLink) {
	n = u.preObj
	return
}

func (u *UserInfo) DLSetPreObj(n datas.IDoubleLink) {
	u.preObj = n
}

func (u *UserInfo) DLGetNextObj() (n datas.IDoubleLink) {
	n = u.nextObj
	return
}

func (u *UserInfo) DLSetNextObj(n datas.IDoubleLink) {
	u.nextObj = n
}

//-----------------------------------IDoubleLink implement-------------------------------

func NewUserInfo(h protocolx.SProtocolHead) (u *UserInfo) {
	u = new(UserInfo)

	u.token = h.Token
	u.roleiid = h.RoleId

	u.UpdateLasttime()

	return
}

func (u *UserInfo) UpdateLasttime() {
	u.lastUpdate = utilc.GetTimestamp()
}

func (u *UserInfo) GetRoleIid() int64 {
	return u.roleiid
}

func (u *UserInfo) GetGateIid() int64 {
	return u.token.GetTokenGateIid()
}

func (u *UserInfo) IsActived(tnow int64) bool {
	return ((u.lastUpdate + CHANNEL_USER_ACTIVE_STEP) >= tnow)
}

func (u *UserInfo) SendChatMsg(item *gpro.ChatMessageItem, ch IChannel) {

	msg := &gpro.Chat_ChatMsgNtf{}
	msg.Channel = &gpro.ChatChannelInfo{Type: GetCTypeOfChannelType(ch.GetType()), Channeldid: ch.GetChannelId()}
	nitem := proto.Clone(item)
	msg.Msgs = &gpro.ChatMessageItems{}
	msg.Msgs.Msgs = append(msg.Msgs.Msgs, nitem.(*gpro.ChatMessageItem))

	u.SendNetProtocol(msg)
}

func (u *UserInfo) SendNetProtocol(msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)
	head := pro.WriteHead()
	head.Token = u.token
	head.RoleId = u.roleiid
	head.ToType = int8(service.ServiceType_Gate)
	head.FromType = int8(service.ServiceType_Chat)

	g.SendMsgToRouter(pro)
}

func (u *UserInfo) SendNetProtocolTo(gto service.ServiceType, msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)

	head := pro.WriteHead()
	head.Token = u.token
	head.RoleId = u.roleiid
	head.ToType = int8(gto)
	head.FromType = int8(service.ServiceType_Chat)

	g.SendMsgToRouter(pro)
}
