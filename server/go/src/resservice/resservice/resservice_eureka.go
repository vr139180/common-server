package resservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *ResService) OnServiceRegisted(iid int64) {

	logx.Warnf("<<<<<<<<<<<< res service node:%d online to eureka >>>>>>>>>>>>", iid)

	l.loginSvrs.SIid = iid
	l.homeSvrs.SIid = iid
	l.softTimer.SetRealState(true)
}

func (l *ResService) OnEurekaLosted() {
	logx.Errorf("res service[%d] lost all connectiones to eureka, so res service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *ResService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	//logx.Debugf("xxxxxxxxxxxxx new size:%d del size:%d", len(newiids), len(deliids))
}
