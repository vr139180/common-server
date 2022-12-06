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

package eureka

import "gamelib/service"

// eureka通知消息
type IEurekaNotify interface {
	//服务注册成功
	OnServiceRegisted(iid int64)
	//服务注册丢失，需要重新注册。包括丢失所有注册连接产生的丢失
	OnEurekaLosted()

	//订阅的服务发生变化
	OnServiceChanged(stype service.ServiceType, newiids []*ServiceNodeInfo, deliids []int64)
	//router的balance服务全列表
	OnRouterBalanceNew(stype service.ServiceType, svrs []int64)
}
