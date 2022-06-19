package net

import (
	"cmslib/gnet"
	"friendservice/friends"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"

	"google.golang.org/protobuf/proto"
)

type RouterNetSession struct {
	gnet.BaseNetSession

	friendsCtrl *friends.FriendsCtrl

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo, cctrl *friends.FriendsCtrl) (p *RouterNetSession) {
	p = new(RouterNetSession)
	p.parent = c
	p.nodeInfo = n
	p.friendsCtrl = cctrl

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
	} else if id > int(gpro.FRIEND_PROTYPE_FRIEND_MSG_BEGIN) && id < int(gpro.FRIEND_PROTYPE_FRIEND_MSGALL_END) {
		l.friendsCtrl.ProcessNetCmd(id, pro)
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
