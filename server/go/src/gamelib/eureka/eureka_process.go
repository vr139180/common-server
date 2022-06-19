package eureka

import (
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/server"
	"gamelib/protobuf/gpro"
	"gamelib/service"

	"google.golang.org/protobuf/proto"
)

func (ec *EurekaCluster) timerAutoConnect(now int64, interval int, tid int64) (finish bool) {
	finish = false

	wm := make(map[*eurekaServerNode]void)
	for node := range ec.waitConnections {
		ns := newEurekaSession(ec, node)
		err := ec.tcpServer.Connect(node.ip, node.port, ns)
		if err != nil {
			logx.Errorf("eureka connect to node[ip:%s port:%d] falied", node.ip, node.port)
			wm[node] = setEmptyMemeber
		}
	}

	//替换成新的
	ec.waitConnections = wm

	return
}

func (ec *EurekaCluster) timerSyncEureka(now int64, interval int, tid int64) (finish bool) {
	finish = false

	if !ec.isRegisted() {
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
	ec.SendMessage(esync, true)

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

		ec.SendMessage(sy, false)
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

	if !ec.isRegisted() {
		esess.registToEurekaCenter(ec.mynode.SvrType)
	} else {
		esess.bindToEurekaCenter(ec.mynode.SvrType)
	}
}

func (ec *EurekaCluster) onMthEurekaDisConnected(s gnet.NetSession) {
	esess := s.(*EurekaSession)

	delete(ec.authsConnection, esess)
	delete(ec.eurekaConnections, esess.node.iid)
	delete(ec.waitConnections, esess.node)

	ind := ec.eurekaVector.IndexOf(esess)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	node := esess.node
	if !ec.isRegisted() {
		// 注册未完成
		ec.waitConnections[node] = setEmptyMemeber
	} else {
		// 是否是有效的连接
		_, ok := ec.eurekaNodes[node.iid]
		if ok {
			ec.waitConnections[node] = setEmptyMemeber
		}
	}

	esess.OnClose()

	//如果是注册成功了
	if ec.IsReady() {
		//和cluster全部断裂
		if ec.eurekaVector.Size() == 0 {
			ec.curState = eurekaLosted
			ec.appProxy.OnEurekaLosted()
		}
	}
}

func (ec *EurekaCluster) onMthServiceRegistAck(ns gnet.NetSession, id int, msg proto.Message) {
	es, _ := ns.(*EurekaSession)
	ack, _ := msg.(*gpro.Erk_ServiceRegistAck)

	delete(ec.authsConnection, es)
	delete(ec.eurekaConnections, es.node.iid)
	delete(ec.waitConnections, es.node)

	ind := ec.eurekaVector.IndexOf(es)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	node := es.node
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
		ec.waitConnections[node] = setEmptyMemeber
	}
}

func (ec *EurekaCluster) onMthServiceBindAck(ns gnet.NetSession, id int, msg proto.Message) {
	es, _ := ns.(*EurekaSession)
	ack, _ := msg.(*gpro.Erk_ServiceBindAck)

	delete(ec.authsConnection, es)
	delete(ec.eurekaConnections, es.node.iid)
	delete(ec.waitConnections, es.node)

	ind := ec.eurekaVector.IndexOf(es)
	if ind != -1 {
		ec.eurekaVector.Remove(ind)
	}

	node := es.node
	if ack.GetResult() == 0 {
		//succeess
		ec.eurekaConnections[node.iid] = es
		ec.eurekaVector.Add(es)
	} else {
		//放回wait列表
		_, ok := ec.eurekaNodes[node.iid]
		if ok {
			ec.waitConnections[node] = setEmptyMemeber
		}
	}
}

func (ec *EurekaCluster) onMthEurekaSync(ns gnet.NetSession, id int, msg proto.Message) {
	ack, _ := msg.(*gpro.Erk_EurekaUpdateNtf)

	for _, v := range ack.Online {
		_, ok := ec.eurekaNodes[v.GetIid()]
		if ok {
			continue
		}

		pinfo := newEurekaServerNode(v.Iid, v.Token, v.Ip, int(v.Port))
		ec.eurekaNodes[pinfo.iid] = pinfo

		ec.waitConnections[pinfo] = setEmptyMemeber
	}

	for _, v := range ack.Offline {
		_, ok := ec.eurekaNodes[v]
		if !ok {
			continue
		}

		delete(ec.eurekaNodes, v)
	}
}

func (ec *EurekaCluster) onMthServiceSubScribeAck(ns gnet.NetSession, id int, msg proto.Message) {
	ack, _ := msg.(*gpro.Erk_ServiceSubscribeAck)

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
	if ec.appProxy != nil && (len(newiid) > 0 || len(deliid) > 0) && ec.isRegisted() {
		ec.appProxy.OnServiceChanged(service.ServiceType(stype), newiid, deliid)
	}

	//获取过一次service订阅后发送确认协议，确认注册完成
	if ec.curState == eurekaRegisted {
		confirm := &gpro.Erk_ServiceRegist_Confirm{}
		confirm.Iid = ec.GetMyNode().Iid
		confirm.Token = ec.GetMyNode().Token

		ns.SendMessage(confirm)

		ec.curState = eurekaReady

		if ec.appProxy != nil {
			ec.appProxy.OnServiceRegisted(ec.mynode.Iid)
		}
	}
}
