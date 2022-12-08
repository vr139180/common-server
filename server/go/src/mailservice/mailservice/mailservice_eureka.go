// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package mailservice

import (
	"cmslib/logx"
	"gamelib/eureka"
	"gamelib/service"
)

//----------------------------------eureka IEurekaNotify interface----------------------------
func (l *MailService) OnServiceRegisted(iid int64) {

	logx.Warnf("<<<<<<<<<<<< mail service node:%d online to eureka >>>>>>>>>>>>", iid)

	l.routerSvrs.RealStartTimer(true)
	l.softTimer.SetRealState(true)
}

func (l *MailService) OnEurekaLosted() {
	logx.Errorf("mail service[%d] lost all connectiones to eureka, so mail service will quit", l.eureka.GetMyNode().Iid)

	l.ExitAppOfLostEureka()
}

func (l *MailService) OnServiceChanged(stype service.ServiceType, newiids []*eureka.ServiceNodeInfo, deliids []int64) {
	logx.Debugf("eureka service subscribed change notify type:%d", int(stype))
	if stype == service.ServiceType_ServiceRouter {
		l.routerSvrs.SyncServiceNodes(stype, newiids, deliids)
	}
}
