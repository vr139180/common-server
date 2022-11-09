package net

import (
	"cmslib/gnet"
	"cmslib/protocolx"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
	"mailservice/mailbox"
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
func (l *RouterNetSession) OnRecvMessage(pro *protocolx.NetProtocol) {
	msgid := pro.GetMsgId()
	if msgid == uint16(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, pro)
	} else if msgid > uint16(gpro.MAIL_PROTYPE_MAIL_MSG_BEGIN) && msgid < uint16(gpro.MAIL_PROTYPE_MAIL_MSGALL_END) {
		l.mailboxCtrl.ProcessNetCmd(pro)
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

func (l *RouterNetSession) SendClusterMessage(msg *protocolx.NetProtocol) {
	l.SendMessage(msg)
}
