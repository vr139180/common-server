package datasync

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *DataSync) OnServiceRegisted(iid int64) {
	logx.Warnf("<<<<<<<<<<<< datasync service node:%d online to eureka >>>>>>>>>>>>", iid)
}

func (l *DataSync) OnEurekaLosted() {
	logx.Errorf("datasync service[%d] lost all connectiones to eureka, so res service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *DataSync) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
}
