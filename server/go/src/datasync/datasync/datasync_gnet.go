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
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/protocolx"
	"errors"
	"gamelib/eureka"
)

func (l *DataSync) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *DataSync) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("DataSync is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *DataSync) OnTCPInitComplete() {
	logx.Warnf("<<<<<<<<<<<<DataSync listen at %s:%d>>>>>>>>>>>>", l.configTool.Ip, l.configTool.Port)
}

func (l *DataSync) OnTCPShutdown() {
}

func (l *DataSync) OnTCPOpened(c gnet.Conn) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeConnected(s)
			default:
			}
		} else {
			panic(errors.New("not support other interface, only gnet.NetSession"))
		}
	} else {
		logx.Infof("login service recv client connection request, but service is not ready.")
		c.Close()
	}
	//when accept a new connection, dont process it, util recv the login message.
}

func (l *DataSync) OnTCPClosed(c gnet.Conn, err error) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeDisConnected(s)
			default:
			}
		}
	}
}

func (l *DataSync) OnRecvMessage(pro *protocolx.NetProtocol) {
	s := c.Context()
	if s != nil {
		ns, ok := s.(gnet.NetSession)
		if ok {
			ns.OnRecvMessage(pro)
		}
	} else {
		if !l.eureka.IsReady() {
			c.Close()
			return
		}
	}
}
