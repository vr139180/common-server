package chatservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *ChatService) OnServiceRegisted(iid int64) {
	logx.Infof("chat service[%d] regist to eureka success........", iid)
	l.routerSvrs.RealStartTimer(true)

	l.softTimer.SetRealState(true)
}

func (l *ChatService) OnEurekaLosted() {
	logx.Errorf("chat service[%d] lost all connectiones to eureka, so chat service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *ChatService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	logx.Debugf("eureka service subscribed change notify type:%d", int(stype))
	if stype == service.ServiceType_Router {
		l.routerSvrs.SyncServiceNodes(stype, newiids, deliids)
	}
}
