package datasync

import (
	"gamelib/eureka"
	"gamelib/service"
	"gamelib/xcluster"
)

//------------------------IClusterAppWrapper-------------------
func (l *DataSync) NewNetSession(s service.ServiceType, p *xcluster.ClusterServiceCtrl, node *eureka.ServiceNodeInfo) xcluster.IClusterNetSession {
	return nil
}

func (l *DataSync) GetMyNode() *eureka.ServiceNodeInfo {
	return l.eureka.GetMyNode()
}
