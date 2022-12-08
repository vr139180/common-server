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
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"

	"google.golang.org/protobuf/proto"
)

//home的netsession
type FightRouterNetSession struct {
	gnet.BaseNetSession

	//扩展参数
	extparams map[string]string

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

func NewFightRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo) (l *FightRouterNetSession) {
	l = new(FightRouterNetSession)
	l.parent = c
	l.nodeInfo = n

	return
}

func (l *FightRouterNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

//--------------------gnet.NetSession interface-----------------------
func (l *FightRouterNetSession) OnRecvMessage(id int, pro proto.Message) {
	if id == int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, id, pro)
	}
}

func (l *FightRouterNetSession) OnClose() {
	l.BaseNetSession.OnClose()
}

//---------------------------------------------------------------------

//-----------------------------------xcluster.IClusterNetSession---------------
func (l *FightRouterNetSession) GetTcpConn() gnet.Conn {
	return l.TcpConn
}

func (l *FightRouterNetSession) GetServiceNode() *eureka.ServiceNodeInfo {
	return l.nodeInfo
}

func (l *FightRouterNetSession) GetNetSession() gnet.NetSession {
	return l
}

func (l *FightRouterNetSession) SendClusterMessage(msg proto.Message) {
	l.SendMessage(msg)
}
