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
