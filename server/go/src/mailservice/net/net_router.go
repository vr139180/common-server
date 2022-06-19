package net

import (
	"cmslib/gnet"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
	"mailservice/mailbox"

	"google.golang.org/protobuf/proto"
)

type RouterNetSession struct {
	gnet.BaseNetSession

	mailboxCtrl *mailbox.MailBoxCtrl

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo, cctrl *mailbox.MailBoxCtrl) (p *RouterNetSession) {
	p = new(RouterNetSession)
	p.parent = c
	p.nodeInfo = n
	p.mailboxCtrl = cctrl

	return p
}

func (l *RouterNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

//--------------------gnet.NetSession interface-----------------------
func (l *RouterNetSession) OnRecvMessage(id int, pro proto.Message) {
	if id == int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, id, pro)
	} else if id > int(gpro.MAIL_PROTYPE_MAIL_MSG_BEGIN) && id < int(gpro.MAIL_PROTYPE_MAIL_MSGALL_END) {
		l.mailboxCtrl.ProcessNetCmd(id, pro)
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

func (l *RouterNetSession) SendClusterMessage(msg proto.Message) {
	l.SendMessage(msg)
}
