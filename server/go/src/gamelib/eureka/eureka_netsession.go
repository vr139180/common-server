package eureka

import (
	"cmslib/gnet"
	"cmslib/server"
	"gamelib/protobuf/gpro"
	"gamelib/service"

	"google.golang.org/protobuf/proto"
)

// eureka节点信息描述
type eurekaServerNode struct {
	iid   int64
	token int64
	ip    string
	port  int
}

func newEurekaServerNode(iid int64, token int64, ip string, port int) (n *eurekaServerNode) {
	n = new(eurekaServerNode)

	n.iid = iid
	n.token = token
	n.ip = ip
	n.port = port
	return
}

// eureka的session
type EurekaSession struct {
	gnet.BaseNetSession

	node   *eurekaServerNode
	parent *EurekaCluster
}

// 创建一个eureka session
func newEurekaSession(p *EurekaCluster, n *eurekaServerNode) (es *EurekaSession) {
	es = new(EurekaSession)
	es.parent = p
	es.node = n
	es.initMsgMapFun()

	return
}

// 注册协议处理函数
func (e *EurekaSession) initMsgMapFun() {
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_EUREKAUPDATE_NTF), e.parent.onMthEurekaSync)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICEREGIST_ACK), e.parent.onMthServiceRegistAck)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICEBIND_ACK), e.parent.onMthServiceBindAck)
	e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_SERVICESUBSCRIBE_ACK), e.parent.onMthServiceSubScribeAck)
}

func (e *EurekaSession) registToEurekaCenter(t service.ServiceType) {
	msg := &gpro.Erk_ServiceRegistReq{}
	msg.SvrType = int32(t)
	msg.Ip = e.parent.mynode.Ip
	msg.Port = int32(e.parent.mynode.Port)
	if e.parent.mynode.Exts != nil && len(e.parent.mynode.Exts) > 0 {
		msg.Exts = e.parent.mynode.Exts
	}

	e.SendMessage(msg)
}

func (e *EurekaSession) bindToEurekaCenter(t service.ServiceType) {
	msg := &gpro.Erk_ServiceBindReq{}
	msg.SvrType = int32(t)
	msg.Iid = e.node.iid
	msg.Token = e.node.token

	e.SendMessage(msg)
}

//--------------------gnet.NetSession interface-----------------------
func (e *EurekaSession) OnRecvMessage(id int, pro proto.Message) {
	//logx.Debugf("eureka client recv msg:%d", id)

	switch m := pro.(type) {
	default:
		fun, err := e.GetMsgMapFun(id)
		if err == nil {
			cmd := server.NewNormalNetCmd(e, id, m, fun)
			e.parent.RegistNetCmd(cmd)
		}
	}
}

func (e *EurekaSession) OnClose() {
	e.BaseNetSession.OnClose()
}
