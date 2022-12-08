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

package net

import (
	"chatservice/channel"
	"cmslib/gnet"
	"cmslib/protocolx"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
)

type RouterNetSession struct {
	gnet.BaseNetSession

	channelCtrl *channel.ChannelCtrl

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo, cctrl *channel.ChannelCtrl) (p *RouterNetSession) {
	p = new(RouterNetSession)
	p.parent = c
	p.nodeInfo = n
	p.channelCtrl = cctrl

	return p
}

func (l *RouterNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

func (l *RouterNetSession) OnUserChannelActive(session gnet.NetSession, pro *protocolx.NetProtocol) {
	act := pro.Msg.(*gpro.Chat_UserChannelsActive)

	for _, cc := range act.Channels {
		ctype := channel.GetChannelType(cc.Type)
		if !channel.IsValidateChannelType(ctype) || cc.Channeldid <= 0 {
			continue
		}

		uinfo := channel.NewUserInfo(pro.Head)

		l.channelCtrl.UserActiveChannel(ctype, cc.Channeldid, uinfo)
	}
}

func (l *RouterNetSession) OnUserSay(session gnet.NetSession, pro *protocolx.NetProtocol) {
	say := pro.Msg.(*gpro.Chat_UserMsgSay)
	ctype := channel.GetChannelType(say.GetChannel().Type)
	if !channel.IsValidateChannelType(ctype) || say.GetChannel().Channeldid <= 0 {
		return
	}
	//内容长度过滤
	if len(say.MsgContent) > 216 {
		return
	}

	l.channelCtrl.UserSaySomthing(ctype, say.GetChannel().Channeldid, pro)
}

//--------------------gnet.NetSession interface-----------------------
func (l *RouterNetSession) OnRecvMessage(pro *protocolx.NetProtocol) {
	msgid := pro.GetMsgId()
	if msgid == uint16(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, pro)
	} else if msgid == uint16(gpro.CHAT_PROTYPE_CHAT_USERCHANNELS_ACTIVE) {
		l.OnUserChannelActive(l, pro)
	} else if msgid == uint16(gpro.CHAT_PROTYPE_CHAT_USERMSG_SAY) {
		l.OnUserSay(l, pro)
	}
}

func (l *RouterNetSession) OnClose() {
	l.BaseNetSession.OnClose()
}

//---------------------------------------------------------------------

//-----------------------------------xcluster.IClusterNetSession---------------
func (l *RouterNetSession) GetTcpConn() gnet.Conn {
	return l.TcpConn
}

func (l *RouterNetSession) GetServiceNode() *eureka.ServiceNodeInfo {
	return l.nodeInfo
}

func (l *RouterNetSession) GetNetSession() gnet.NetSession {
	return l
}

func (l *RouterNetSession) SendClusterMessage(pro *protocolx.NetProtocol) {
	l.SendMessage(pro)
}
