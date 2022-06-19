package friendservice

import (
	"friendservice/net"
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
)

//------------------------IClusterAppWrapper-------------------
func (l *FriendService) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {

	if s == service.ServiceType_Router {
		ns := net.NewRouterNetSession(p, node, l.friendsCtrl)
		return ns
	}

	return nil
}

func (l *FriendService) GetMyNode() *eureka.ServiceNodeInfo {
	return l.eureka.GetMyNode()
}
