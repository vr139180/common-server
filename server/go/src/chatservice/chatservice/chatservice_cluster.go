package chatservice

import (
	"chatservice/net"
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
)

//------------------------IClusterAppWrapper-------------------
func (l *ChatService) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {

	if s == service.ServiceType_Router {
		ns := net.NewRouterNetSession(p, node, l.channelCtrl)
		return ns
	}

	return nil
}

func (l *ChatService) GetMyNode() *eureka.ServiceNodeInfo {
	return l.eureka.GetMyNode()
}
