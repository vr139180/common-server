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
		wm := make(map[int64]*eurekaServerNode)
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

func (ec *EurekaCluster) timerSyncEureka(now int64, interval int, tid int64) (finish bool) {
	finish = false

	if !ec.IsRegisted() {
		return
	}

	// sync eureka
	esync := &gpro.Erk_EurekaSync{}
	esync.Myiid = ec.mynode.Iid

	eids := []int64{}
	for i := range ec.eurekaNodes {
		eids = append(eids, i)
	}
	esync.Exists = eids
	ec.SendNetProtocol(esync, true)

	//sync service
	{
		sy := &gpro.Erk_ServiceSubscribeReq{}
		sy.Myiid = ec.mynode.Iid
		sy.Mysvrtype = int32(ec.mynode.SvrType)
		sy.Myip = ec.mynode.Ip
		sy.Myport = int32(ec.mynode.Port)

		sbs := []*gpro.Erk_ServiceSubscribeReqSvrinfo{}
		for v := range ec.subscribes {
			s := &gpro.Erk_ServiceSubscribeReqSvrinfo{}
			s.SvrType = int32(v)
			//get exist nodes
			nods := ec.getServiceNodesByType(v)
			if nods != nil && len(nods) > 0 {
				sid := []int64{}
				for i := range nods {
					sid = append(sid, int64(i))
				}
				s.Exits = sid
			}

			sbs = append(sbs, s)
		}
		if len(sbs) > 0 {
			sy.SvrType = sbs
		}

		ec.SendNetProtocol(sy, false)
	}

	return
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
	delete(ec.eurekaConnections, esess.GetIid())

	ind := ec.eurekaVector.IndexOf(esess)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	node = node.Clone()
	if !ec.IsRegisted() {
		// 注册未完成
		ec.waitConnections[node.iid] = node
	} else {
		// 是否是有效的连接
		_, ok := ec.eurekaNodes[node.iid]
		if ok {
			ec.waitConnections[node.iid] = node
		}
	}

	esess.OnClose()

	logx.Debugf("----+++++++eureka:%d disconnection, connections:%d vector:%d waits:%d auths:%d", esess.GetIid(), len(ec.eurekaConnections), ec.eurekaVector.Size(), len(ec.waitConnections), len(ec.authsConnection))

	//如果是注册成功了
	if ec.IsReady() {
		//和cluster全部断裂
		if ec.eurekaVector.Size() == 0 {
			ec.curState = eurekaLosted
			ec.appProxy.OnEurekaLosted()
		}
	}
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
		ec.eurekaNodes[node.iid] = node

		ec.eurekaConnections[node.iid] = es
		ec.eurekaVector.Add(es)

		ec.curState = eurekaRegisted

		ec.mynode.Iid = ack.GetIid()
		ec.mynode.Token = ack.GetToken()

	} else {
		//放回wait列表
		node = node.Clone()
		ec.waitConnections[node.iid] = node
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

func (ec *EurekaCluster) onMthEurekaSync(ns gnet.NetSession, pro *protocolx.NetProtocol) {
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

func (ec *EurekaCluster) onMthServiceSubScribeAck(ns gnet.NetSession, pro *protocolx.NetProtocol) {
	ack, _ := pro.Msg.(*gpro.Erk_ServiceSubscribeAck)

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
