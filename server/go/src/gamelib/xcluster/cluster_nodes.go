package xcluster

import (
	"cmslib/gnet"
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/service"

	"google.golang.org/protobuf/proto"
)

type syncServiceCommand struct {
	stype   service.ServiceType
	newiids []*eureka.ServiceNodeInfo
	deliids []int64

	fun func(service.ServiceType, []*eureka.ServiceNodeInfo, []int64)
}

func (s *syncServiceCommand) Run() {
	s.fun(s.stype, s.newiids, s.deliids)
}

func (c *ClusterServiceCtrl) SyncServiceNodes(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	cmd := &syncServiceCommand{stype: stype, newiids: newiids, deliids: deliids}
	cmd.fun = c.syncServiceNodesOnSameCoGoutine
	c.ch <- cmd
}

func (c *ClusterServiceCtrl) syncServiceNodesOnSameCoGoutine(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	if stype != c.targetSType {
		return
	}

	for _, d := range deliids {
		_, ok := c.serviceNodes[d]
		if ok {
			delete(c.serviceNodes, d)
		}
	}

	for _, n := range newiids {
		_, ok := c.serviceNodes[n.Iid]
		if !ok {
			c.serviceNodes[n.Iid] = n

			c.waitConnections[n] = setEmptyMemeber
		}
	}

}

func (c *ClusterServiceCtrl) timerAutoConnect(now int64, interval int, tid int64) (finish bool) {
	finish = false

	if len(c.waitConnections) == 0 {
		return
	}

	wm := make(map[*eureka.ServiceNodeInfo]void)
	for node := range c.waitConnections {
		n := c.appProxy.NewNetSession(c.targetSType, c, node)
		ns := n.(gnet.NetSession)
		err := c.tcpServer.Connect(node.Ip, node.Port, ns)
		if err != nil {
			logx.Errorf("connect to cluster node[ip:%s port:%d] falied", node.Ip, node.Port)
			wm[node] = setEmptyMemeber
		} else {
			logx.Infof("beginning connect to cluster node[ip:%s port:%d] ...........", node.Ip, node.Port)
		}
	}

	//替换成新的
	c.waitConnections = wm

	return
}

//------------------------------------------------------------------------------
type resNodeConnectCommand struct {
	ns IClusterNetSession

	fun func(IClusterNetSession)
}

func (s *resNodeConnectCommand) Run() {
	s.fun(s.ns)
}

func (c *ClusterServiceCtrl) OnResNodeConnected(s IClusterNetSession) {
	cmd := &resNodeConnectCommand{ns: s}
	cmd.fun = c.onMthServiceConnected

	c.ch <- cmd
}

func (c *ClusterServiceCtrl) onMthServiceConnected(s IClusterNetSession) {
	c.authConnections[s] = setEmptyMemeber

	ns := s.(gnet.NetSession)

	req := &gpro.Svr_ServiceBindServiceReq{}
	req.SvrType = int32(c.mynode.SvrType)
	req.Myiid = c.mynode.Iid
	req.Mytoken = c.mynode.Token
	req.Myexts = c.mynode.CloneExtParams()

	tonode := s.GetServiceNode()
	req.Toiid = tonode.Iid
	req.Totoken = tonode.Token

	ns.SendMessage(req)
}

func (c *ClusterServiceCtrl) OnResNodeDisConnected(s IClusterNetSession) {
	cmd := &resNodeConnectCommand{ns: s}
	cmd.fun = c.onMthServiceDisConnected

	c.ch <- cmd
}

func (c *ClusterServiceCtrl) onMthServiceDisConnected(s IClusterNetSession) {

	node := s.GetServiceNode()

	_, ok := c.serviceNodes[node.Iid]
	if ok {
		c.waitConnections[node] = setEmptyMemeber
	}

	delete(c.authConnections, s)
	delete(c.waitConnections, node)

	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()

	delete(c.serviceConnections, node.Iid)
	ind := c.serviceVector.IndexOf(s)
	if ind != -1 {
		c.serviceVector.Remove(ind)
	}

	logx.Warnf("disconnect from cluster node[ip:%s port:%d]", node.Ip, node.Port)
}

//-------------------------------------------------------------------------
func (c *ClusterServiceCtrl) OnResNodeRegistAck(ns IClusterNetSession, id int, pro proto.Message) {
	cmd := NewClusterNetCmd(ns, id, pro, c.OnMthResNodeRegistAck)
	c.ch <- cmd
}

func (c *ClusterServiceCtrl) OnMthResNodeRegistAck(s IClusterNetSession, id int, m proto.Message) {
	ack, _ := m.(*gpro.Svr_ServiceBindServiceAck)

	node := s.GetServiceNode()
	delete(c.authConnections, s)
	delete(c.waitConnections, node)

	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()

	delete(c.serviceConnections, node.Iid)

	ind := c.serviceVector.IndexOf(s)
	if ind != -1 {
		c.serviceVector.Remove(ind)
	}

	if ack.GetResult() == 0 {
		//succeess
		c.serviceConnections[node.Iid] = s
		c.serviceVector.Add(s)

		logx.Infof("connect to cluster node[ip:%s port:%d] success", node.Ip, node.Port)
	} else {
		//放回wait列表
		_, ok := c.serviceNodes[node.Iid]
		if ok {
			c.waitConnections[node] = setEmptyMemeber
		}

		logx.Infof("connect to cluster node[ip:%s port:%d] failed", node.Ip, node.Port)
	}
}
