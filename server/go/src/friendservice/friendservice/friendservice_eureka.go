package friendservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *FriendService) OnServiceRegisted(iid int64) {
	logx.Infof("friend service[%d] regist to eureka success........", iid)
	l.routerSvrs.RealStartTimer(true)

	l.softTimer.SetRealState(true)
}

func (l *FriendService) OnEurekaLosted() {
	logx.Errorf("friend service[%d] lost all connectiones to eureka, so friend service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *FriendService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	logx.Debugf("eureka service subscribed change notify type:%d", int(stype))
	if stype == service.ServiceType_Router {
		l.routerSvrs.SyncServiceNodes(stype, newiids, deliids)
	}
}
