package matchmaking

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *MatchMakingService) OnServiceRegisted(iid int64) {
	logx.Infof("matchmaking service[%d] regist to eureka success........", iid)

	l.fightRouterSvrs.RealStartTimer(true)

	l.softTimer.SetRealState(true)
}

func (l *MatchMakingService) OnEurekaLosted() {
	logx.Errorf("matchmaking service[%d] lost all connectiones to eureka, so res service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *MatchMakingService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	logx.Debugf("eureka service subscribed change notify type:%d", int(stype))
	if stype == service.ServiceType_FightRouter {
		l.fightRouterSvrs.SyncServiceNodes(stype, newiids, deliids)
	}
}
