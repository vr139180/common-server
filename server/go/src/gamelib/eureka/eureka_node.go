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

//-------------------------------------------------------- 服务节点信息--------------------------------------
type ServiceNodeInfo struct {
	Iid     int64
	Token   int64
	SvrType service.ServiceType
	Exts    map[string]string

	//node的地址
	Ip   string
	Port int
	//是否router服务
	IsRouter bool
}

func (sn *ServiceNodeInfo) CloneExtParams() (r map[string]string) {
	r = make(map[string]string)
	if sn.Exts != nil && len(sn.Exts) > 0 {
		for k, v := range sn.Exts {
			r[k] = v
		}
	}

	return
}

func (sn *ServiceNodeInfo) GetExtParamByKey(k string) (v string, ok bool) {
	if sn.Exts == nil {
		ok = false
		return
	}

	v, ok = sn.Exts[k]
	return
}

// 构建一个service节点描述信息
func newServiceNodeInfo(s service.ServiceType, tid int64, token int64, ip string, port int, exts map[string]string, isrouter bool) (n *ServiceNodeInfo) {
	n = new(ServiceNodeInfo)
	n.SvrType = s
	n.Iid = tid
	n.Token = token
	n.Ip = ip
	n.Port = port
	n.Exts = exts
	n.IsRouter = isrouter

	return n
}

//------------------------------------------------------------eureka节点信息描述-----------------------------------------------
type EurekaNodeInfo struct {
	iid   int64
	token int64
	ip    string
	port  int
	//master节点标志
	ismaster bool
}

func (s *EurekaNodeInfo) Clone() *EurekaNodeInfo {
	cp := *s
	return &cp
}

func newEurekaNodeInfo(iid int64, token int64, ip string, port int, ismaster bool) (n *EurekaNodeInfo) {
	n = new(EurekaNodeInfo)

	n.iid = iid
	n.token = token
	n.ip = ip
	n.port = port
	n.ismaster = ismaster

	return
}

func newEurekaNodeInfo2(node map[string]interface{}) (n *EurekaNodeInfo) {
	n = nil

	id, ok := node["iid"]
	if !ok {
		return
	}
	ip, ok := node["ip"]
	if !ok {
		return
	}
	port, ok := node["port"]
	if !ok {
		return
	}
	token, ok := node["token"]
	if !ok {
		return
	}

	n = new(EurekaNodeInfo)
	n.iid = id.(int64)
	n.ip = ip.(string)
	n.port = port.(int)
	n.token = token.(int64)
	n.ismaster = true

	return
}
