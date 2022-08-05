package matchmaking

import (
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
	"matchmaking/net"
)

//------------------------IClusterAppWrapper-------------------
func (l *MatchMakingService) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {

	if s == service.ServiceType_FightRouter {
		ns := net.NewFightRouterNetSession(p, node)
		return ns
	}

	return nil
}
