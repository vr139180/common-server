package loginservice

import (
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
	"loginservice/microsvr"
)

//------------------------IClusterAppWrapper-------------------
func (l *LoginService) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {
	if s == service.ServiceType_Res {
		ns := microsvr.NewResNetSession(p, node)
		return ns
	}

	return nil
}

func (l *LoginService) GetMyNode() *eureka.ServiceNodeInfo {
	return l.eureka.GetMyNode()
}
