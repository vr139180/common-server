package loginservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *LoginService) OnServiceRegisted(iid int64) {

	logx.Warnf("<<<<<<<<<<<< login service node:%d online to eureka >>>>>>>>>>>>", iid)

	l.resCtrl.RealStartTimer(true)
}

func (l *LoginService) OnEurekaLosted() {
	logx.Errorf("login service[%d] lost all connectiones to eureka, so res service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *LoginService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	if stype == service.ServiceType_Res {
		l.resCtrl.SyncServiceNodes(stype, newiids, deliids)
	}
}
