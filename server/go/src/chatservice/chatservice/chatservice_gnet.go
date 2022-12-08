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

package chatservice

import (
	"chatservice/net"
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/protocolx"
	"errors"
	"gamelib/eureka"

	_ "google.golang.org/protobuf/proto"
)

func (l *ChatService) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *ChatService) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("Chat Service is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *ChatService) OnTCPInitComplete() {
	logx.Warnf("<<<<<<<<<<<<Chat Service listen at %s:%d>>>>>>>>>>>>", l.configTool.Ip, l.configTool.Port)
}

func (l *ChatService) OnTCPShutdown() {
}

func (l *ChatService) OnTCPOpened(c gnet.Conn) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeConnected(s)
			case *net.RouterNetSession:
				l.routerSvrs.OnResNodeConnected(s)
			default:
			}
		} else {
			panic(errors.New("not support other interface, only gnet.NetSession"))
		}
	} else {
		c.Close()
	}
}

func (l *ChatService) OnTCPClosed(c gnet.Conn, err error) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeDisConnected(s)
			case *net.RouterNetSession:
				l.routerSvrs.OnResNodeDisConnected(s)
			default:
			}
		}
	}
}

func (l *ChatService) OnRecvMessage(c gnet.Conn, pro *protocolx.NetProtocol) {
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
