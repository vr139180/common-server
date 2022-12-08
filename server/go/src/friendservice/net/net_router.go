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
	"cmslib/gnet"
	"cmslib/protocolx"
	"friendservice/friends"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
)

type RouterNetSession struct {
	gnet.BaseNetSession

	friendsCtrl *friends.FriendsCtrl

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo, cctrl *friends.FriendsCtrl) (p *RouterNetSession) {
	p = new(RouterNetSession)
	p.parent = c
	p.nodeInfo = n
	p.friendsCtrl = cctrl

	return p
}

func (l *RouterNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

//--------------------gnet.NetSession interface-----------------------
func (l *RouterNetSession) OnRecvMessage(pro *protocolx.NetProtocol) {
	msgid := pro.GetMsgId()
	if msgid == uint16(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, pro)
	} else if msgid > uint16(gpro.FRIEND_PROTYPE_FRIEND_MSG_BEGIN) && msgid < uint16(gpro.FRIEND_PROTYPE_FRIEND_MSGALL_END) {
		l.friendsCtrl.ProcessNetCmd(pro)
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
