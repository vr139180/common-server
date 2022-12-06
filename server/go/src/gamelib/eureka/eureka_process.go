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
	"cmslib/logx"
	"cmslib/protocolx"
	"cmslib/server"
	"gamelib/protobuf/gpro"
	"gamelib/service"
)

func (ec *EurekaCluster) timerAutoConnect(now int64, interval int, tid int64) (finish bool) {
	finish = false

	if len(ec.waitConnections) == 0 {
		return
	}

	if (!ec.IsRegisted()) || ec.IsReady() {
		wm := make(map[int64]*EurekaNodeInfo)
		for _, node := range ec.waitConnections {
			ns := newEurekaSession(ec, node)
			err := ns.ConnectTo()
			if err != nil {
				logx.Errorf(">>>>>>> connect to eureka node[ip:%s port:%d] falied", ns.GetIp(), ns.GetPort())
				//未完成注册的
				if !ec.IsRegisted() {
					wm[node.iid] = node
				} else if ec.IsExistEurekaNode(node.iid) {
					//存在于eurekanode的
					wm[node.iid] = node
				}
			}
		}

		//替换成新的
		ec.waitConnections = wm
	}

	return
}

func (ec *EurekaCluster) timerServiceAlive(now int64, interval int, tid int64) (finish bool) {
	finish = false

	if !ec.IsRegisted() || !ec.HasEurekaMasterNode() {
		return
	}

	ntf := &gpro.Svr_LiveTickNtf{}
	ec.SendToMaster(ntf)

	return
}

func (ec *EurekaCluster) timerRouterAutoConfig(now int64, interval int, tid int64) (finish bool) {
	finish = true

	ec.curState = eurekaReady

	req := &gpro.Erk_RouterOnlineReq{}
	req.Myiid = ec.mynode.Iid
	req.Mysvrtype = int32(ec.mynode.SvrType)

	ec.SendToMaster(req)

	ec.appProxy.OnServiceRegisted(ec.mynode.Iid)
}

// 一个节点连接成功
func (ec *EurekaCluster) OnEurekaNodeConnected(node *EurekaSession) {
	cmd := server.NewNetSessionCmd(node, ec.onMthEurekaConnected)
	ec.RegistNetCmd(cmd)
}

//一个节点断开连接
func (ec *EurekaCluster) OnEurekaNodeDisConnected(node *EurekaSession) {
	cmd := server.NewNetSessionCmd(node, ec.onMthEurekaDisConnected)
	ec.RegistNetCmd(cmd)
}

func (ec *EurekaCluster) onMthEurekaConnected(s gnet.NetSession) {
	esess := s.(*EurekaSession)
	ec.authsConnection[esess] = setEmptyMemeber

	if !ec.IsRegisted() {
		esess.registToEurekaCenter(ec.mynode.SvrType)
	} else {
		esess.bindToEurekaCenter(ec.mynode.SvrType)
	}
}

func (ec *EurekaCluster) onMthEurekaDisConnected(s gnet.NetSession) {
	esess := s.(*EurekaSession)

	node := esess.node

	delete(ec.waitConnections, node.iid)
	delete(ec.authsConnection, esess)

	//remove from online
	delete(ec.eurekaConnections, esess.GetIid())
	ind := ec.eurekaVector.IndexOf(esess)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	node = node.Clone()

	if ec.IsRegisted() {
		if ec.masterLink == esess {
			ec.masterLink = nil
			logx.Debugf("me lost connection to sEureka[iid:%d ip:%s port:%d] master node", node.iid, node.ip, node.port)
		}

		if ec.IsExistEurekaNode(node.iid) {
			ec.waitConnections[node.iid] = node
		}

		if ec.eurekaVector.Size() == 0 {
			ec.curState = eurekaLosted
			ec.appProxy.OnEurekaLosted()
		}

	} else {
		ec.waitConnections[node.iid] = node
	}

	esess.Close()

	logx.Debugf("----+++++++eureka:%d disconnection, connections:%d vector:%d waits:%d auths:%d", esess.GetIid(), len(ec.eurekaConnections), ec.eurekaVector.Size(), len(ec.waitConnections), len(ec.authsConnection))
}

func (ec *EurekaCluster) onMthServiceRegistAck(ns gnet.NetSession, pro *protocolx.NetProtocol) {
	es, _ := ns.(*EurekaSession)

	ack, _ := pro.Msg.(*gpro.Erk_ServiceRegistAck)

	node := es.node

	delete(ec.waitConnections, node.iid)
	delete(ec.authsConnection, es)

	delete(ec.eurekaConnections, es.GetIid())
	ind := ec.eurekaVector.IndexOf(es)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	if ack.GetResult() == 0 {
		//succeess
		node.iid = ack.GetEurekaiid()
		node.token = ack.GetEurekatoken()
		node.ismaster = true

		ec.mynode.Iid = ack.GetIid()
		ec.mynode.Token = ack.GetToken()

		ec.masterEurekaIid = node.iid
		ec.masterEurekaToken = node.token
		ec.masterLink = es

		ec.eurekaNodes[node.iid] = node.Clone()
		ec.eurekaConnections[node.iid] = es
		ec.eurekaVector.Add(es)

		ec.curState = eurekaRegisted

		//subscribe to master
		ec.SubscribeToMasterNode()

		if ec.mynode.IsRouter {
			ec.timerContainer.AddTimer(3*1000, ec.timerRouterAutoConfig)
		} else {
			ec.curState = eurekaReady
			ec.appProxy.OnServiceRegisted(ec.mynode.Iid)
		}

	} else {
		//放回wait列表
		//node = node.Clone()
		//ec.waitConnections[node.iid] = node
		es.Close()
	}

	logx.Debugf("-------regist result, eureka connections:%d vector:%d waits:%d auths:%d", len(ec.eurekaConnections), ec.eurekaVector.Size(), len(ec.waitConnections), len(ec.authsConnection))
}

func (ec *EurekaCluster) onMthServiceBindAck(ns gnet.NetSession, pro *protocolx.NetProtocol) {
	es, _ := ns.(*EurekaSession)
	ack, _ := pro.Msg.(*gpro.Erk_ServiceBindAck)

	node := es.node

	delete(ec.waitConnections, node.iid)
	delete(ec.authsConnection, es)
	delete(ec.eurekaConnections, es.node.iid)

	ind := ec.eurekaVector.IndexOf(es)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	if ack.GetResult() == 0 {
		//succeess
		ec.eurekaConnections[node.iid] = es
		ec.eurekaVector.Add(es)
	} else {
		//放回wait列表
		_, ok := ec.eurekaNodes[node.iid]
		if ok {
			ec.waitConnections[node.iid] = node
		}
	}

	logx.Debugf("-------bind result, eureka connections:%d vector:%d waits:%d auths:%d", len(ec.eurekaConnections), ec.eurekaVector.Size(), len(ec.waitConnections), len(ec.authsConnection))
}

func (ec *EurekaCluster) onMthEurekaUpdateNtf(ns gnet.NetSession, pro *protocolx.NetProtocol) {
	ack, _ := pro.Msg.(*gpro.Erk_EurekaUpdateNtf)

	for _, v := range ack.Online {
		_, ok := ec.eurekaNodes[v.GetIid()]
		if ok {
			continue
		}

		pinfo := newEurekaServerNode(v.Iid, v.Token, v.Ip, int(v.Port))
		ec.eurekaNodes[pinfo.iid] = pinfo

		if !ec.IsOnlineEurekaNode(pinfo.iid) {
			ec.waitConnections[pinfo.iid] = pinfo
		}
	}

	for _, v := range ack.Offline {
		logx.Debugf("------ sync eureka offline:%d", v)
		_, ok := ec.eurekaNodes[v]
		if !ok {
			continue
		}

		delete(ec.eurekaNodes, v)
		delete(ec.waitConnections, v)
		d, ok := ec.eurekaConnections[v]
		if ok {
			delete(ec.authsConnection, d)
			delete(ec.eurekaConnections, v)
			//force close
			d.Close()
		}
	}
}

func (ec *EurekaCluster) onMthRouterOnlineNtf(ns gnet.NetSession, pro *protocolx.NetProtocol) {

}

func (ec *EurekaCluster) onMthMasterChaneNtf(ns gnet.NetSession, pro *protocolx.NetProtocol) {

}

func (ec *EurekaCluster) onMthRouterSubScribeNtf(ns gnet.NetSession, pro *protocolx.NetProtocol) {

}

func (ec *EurekaCluster) onMthServiceSubScribeNtf(ns gnet.NetSession, pro *protocolx.NetProtocol) {
	ack, _ := pro.Msg.(*gpro.Erk_ServiceSubscribeNtf)

	stype := int(ack.SvrType)
	nodes := ec.getServiceNodesByType(stype)
	newiid := []*ServiceNodeInfo{}

	for _, v := range ack.Online {
		_, ok := nodes[v.Iid]
		if ok {
			continue
		}

		var exts map[string]string
		if v.Exts != nil && len(v.Exts) > 0 {
			exts = make(map[string]string)
			for k, v := range v.Exts {
				exts[k] = v
			}
		}

		ps := newServiceNodeInfo(service.ServiceType(stype), v.Iid, v.Token, v.Ip, int(v.Port), exts)
		nodes[ps.Iid] = ps

		newiid = append(newiid, ps)
	}

	deliid := []int64{}
	for _, v := range ack.Offline {
		_, ok := nodes[v]
		if !ok {
			continue
		}

		deliid = append(deliid, v)
		delete(nodes, v)
	}

	// notify application
	if ec.appProxy != nil && (len(newiid) > 0 || len(deliid) > 0) && ec.IsRegisted() {
		ec.appProxy.OnServiceChanged(service.ServiceType(stype), newiid, deliid)
	}

	//获取过一次service订阅后发送确认协议，确认注册完成
	if ec.curState == eurekaRegisted {
		confirm := &gpro.Erk_ServiceRegist_Confirm{}
		confirm.Iid = ec.GetMyNode().Iid
		confirm.Token = ec.GetMyNode().Token

		ens, _ := ns.(*EurekaSession)
		ens.SendNetProtocol(confirm)

		ec.curState = eurekaReady

		if ec.appProxy != nil {
			ec.appProxy.OnServiceRegisted(ec.mynode.Iid)
		}
	}
}
