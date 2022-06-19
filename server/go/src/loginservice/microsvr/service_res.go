package microsvr

import (
	"cmslib/gnet"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
	"loginservice/g"

	"google.golang.org/protobuf/proto"
)

//res service的连接
type ResNetSession struct {
	gnet.BaseNetSession

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewResNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo) (p *ResNetSession) {
	p = new(ResNetSession)
	p.parent = c
	p.nodeInfo = n

	p.initMsgMapFun()

	return p
}

// 注册协议处理函数
func (r *ResNetSession) initMsgMapFun() {
	//e.RegistMsgMapFun(int(gpro.ERK_PROTYPE_ERK_EUREKAUPDATE_NTF), e.parent.onMthEurekaSync)
}

//--------------------gnet.NetSession interface-----------------------
func (l *ResNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

func (l *ResNetSession) OnRecvMessage(id int, pro proto.Message) {
	if id == int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		l.parent.OnResNodeRegistAck(l, id, pro)
	} else {
		g.RegistProcessNetCmd(l, gpro.ERK_PROTYPE(id), pro)
	}
}

func (l *ResNetSession) OnClose() {
	l.BaseNetSession.OnClose()
}

//---------------------------------------------------------------------

//-----------------------------------xcluster.IClusterNetSession---------------
func (l *ResNetSession) GetTcpConn() gnet.Conn {
	return l.TcpConn
}

func (l *ResNetSession) GetServiceNode() *eureka.ServiceNodeInfo {
	return l.nodeInfo
}

func (l *ResNetSession) GetNetSession() gnet.NetSession {
	return l
}

func (l *ResNetSession) SendClusterMessage(msg proto.Message) {
	l.SendMessage(msg)
}
