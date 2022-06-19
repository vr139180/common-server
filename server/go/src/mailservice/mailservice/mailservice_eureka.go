package mailservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *MailService) OnServiceRegisted(iid int64) {
	logx.Infof("mail service[%d] regist to eureka success........", iid)
	l.routerSvrs.RealStartTimer(true)

	l.softTimer.SetRealState(true)
}

func (l *MailService) OnEurekaLosted() {
	logx.Errorf("mail service[%d] lost all connectiones to eureka, so mail service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *MailService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	logx.Debugf("eureka service subscribed change notify type:%d", int(stype))
	if stype == service.ServiceType_Router {
		l.routerSvrs.SyncServiceNodes(stype, newiids, deliids)
	}
}
