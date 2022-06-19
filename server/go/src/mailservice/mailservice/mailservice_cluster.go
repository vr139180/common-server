package mailservice

import (
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
	"mailservice/net"
)

//------------------------IClusterAppWrapper-------------------
func (l *MailService) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {

	if s == service.ServiceType_Router {
		ns := net.NewRouterNetSession(p, node, l.mailboxCtrl)
		return ns
	}

	return nil
}

func (l *MailService) GetMyNode() *eureka.ServiceNodeInfo {
	return l.eureka.GetMyNode()
}
