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

package eureka

import (
	"cmslib/gnet"
	"cmslib/protocolx"
	"cmslib/server"
	"gamelib/protobuf/gpro"
	"gamelib/service"

	"google.golang.org/protobuf/proto"
)

// eureka的session
type EurekaSession struct {
	gnet.BaseNetSession

	node   *EurekaNodeInfo
	parent *EurekaCluster
}

// 创建一个eureka session
func newEurekaSession(p *EurekaCluster, n *EurekaNodeInfo) (es *EurekaSession) {
	es = new(EurekaSession)
	es.parent = p
	es.node = n
	es.initMsgMapFun()

	return
}

// 注册协议处理函数
func (e *EurekaSession) initMsgMapFun() {
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_EUREKAUPDATE_NTF), e.parent.onMthEurekaUpdateNtf)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_NTF), e.parent.onMthServiceSubScribeNtf)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_ROUTERSUBSCRIBE_NTF), e.parent.onMthRouterSubScribeNtf)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_MASTERCHANGE_NTF), e.parent.onMthMasterChaneNtf)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_ROUTERSUBSCRIBE_NTF), e.parent.onMthRouterOnlineNtf)

	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_ACK), e.parent.onMthServiceRegistAck)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_ACK), e.parent.onMthServiceBindAck)
}

func (e *EurekaSession) GetIid() int64 {
	return e.node.iid
}

func (e *EurekaSession) GetIpPort() (string, int) {
	return e.node.ip, e.node.port
}

func (e *EurekaSession) GetIp() string {
	return e.node.ip
}

func (e *EurekaSession) GetPort() int {
	return e.node.port
}

func (e *EurekaSession) ConnectTo() error {
	return e.parent.tcpServer.Connect(e.node.ip, e.node.port, e)
}

func (e *EurekaSession) registToEurekaCenter(t service.ServiceType) {
	msg := &gpro.Erk_ServiceRegistReq{}
	msg.SvrType = int32(t)
	msg.Ip = e.parent.mynode.Ip
	msg.Port = int32(e.parent.mynode.Port)
	if e.parent.mynode.Exts != nil && len(e.parent.mynode.Exts) > 0 {
		msg.Exts = e.parent.mynode.Exts
	}
	msg.Isrouter = e.parent.mynode.IsRouter

	e.SendToEureka(msg)
}

func (e *EurekaSession) bindToEurekaCenter(t service.ServiceType) {
	msg := &gpro.Erk_ServiceBindReq{}
	msg.SvrType = int32(t)
	msg.Iid = e.parent.mynode.Iid
	msg.Token = e.parent.mynode.Token
	msg.Eurekatoken = e.parent.masterEurekaToken

	e.SendToEureka(msg)
}

func (e *EurekaSession) SendToEureka(msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)

	head := pro.WriteHead()
	head.FromType = int8(e.parent.GetMyNode().SvrType)
	head.ToType = int8(gpro.ERK_SERVICETYPE_ERK_SERVICE_EUREKA)

	e.SendMessage(pro)
}

//--------------------gnet.NetSession interface-----------------------
func (e *EurekaSession) OnRecvMessage(pro *protocolx.NetProtocol) {

	fun, err := e.GetMsgMapFun(int(pro.GetMsgId()))

	if err == nil {
		cmd := server.NewNormalNetCmd(e, pro, fun)
		e.parent.RegistNetCmd(cmd)
	}
}

func (e *EurekaSession) OnClose() {
	e.BaseNetSession.OnClose()
}
