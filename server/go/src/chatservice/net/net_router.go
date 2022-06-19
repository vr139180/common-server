package net

import (
	"chatservice/channel"
	"cmslib/gnet"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"

	"google.golang.org/protobuf/proto"
)

type RouterNetSession struct {
	gnet.BaseNetSession

	channelCtrl *channel.ChannelCtrl

	parent   *xcluster.ClusterServiceCtrl
	nodeInfo *eureka.ServiceNodeInfo
}

// 构建一个和tcp连接关联的session处理对象
func NewRouterNetSession(c *xcluster.ClusterServiceCtrl, n *eureka.ServiceNodeInfo, cctrl *channel.ChannelCtrl) (p *RouterNetSession) {
	p = new(RouterNetSession)
	p.parent = c
	p.nodeInfo = n
	p.channelCtrl = cctrl

	return p
}

func (l *RouterNetSession) GetExtParamByKey(k string) (v string, ok bool) {
	v, ok = l.nodeInfo.GetExtParamByKey(k)
	return
}

func (l *RouterNetSession) OnUserChannelActive(session gnet.NetSession, iid int, msg proto.Message) {
	act := msg.(*gpro.Chat_UserChannelsActive)

	for _, cc := range act.Channels {
		ctype := channel.GetChannelType(cc.Type)
		if !channel.IsValidateChannelType(ctype) || cc.Channeldid <= 0 {
			continue
		}

		uinfo := channel.NewUserInfo(act.GetUtoken().Giduid, act.GetUtoken().Slottoken)

		l.channelCtrl.UserActiveChannel(ctype, cc.Channeldid, uinfo)
	}
}

func (l *RouterNetSession) OnUserSay(session gnet.NetSession, iid int, msg proto.Message) {
	say := msg.(*gpro.Chat_UserMsgSay)
	ctype := channel.GetChannelType(say.GetChannel().Type)
	if !channel.IsValidateChannelType(ctype) || say.GetChannel().Channeldid <= 0 {
		return
	}
	//内容长度过滤
	if len(say.MsgContent) > 216 {
		return
	}

	l.channelCtrl.UserSaySomthing(ctype, say.GetChannel().Channeldid, say)
}

//--------------------gnet.NetSession interface-----------------------
func (l *RouterNetSession) OnRecvMessage(id int, pro proto.Message) {
	if id == int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_ACK) {
		//整合cluster node的注册机制
		l.parent.OnResNodeRegistAck(l, id, pro)
	} else if id == int(gpro.CHAT_PROTYPE_CHAT_USERCHANNELS_ACTIVE) {
		l.OnUserChannelActive(l, id, pro)
	} else if id == int(gpro.CHAT_PROTYPE_CHAT_USERMSG_SAY) {
		l.OnUserSay(l, id, pro)
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
