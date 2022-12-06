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

package server

import (
	"cmslib/gnet"
	"cmslib/protocolx"
)

// 系统命令
type ICommandBase interface {
	Run()
}

type RegistConnectionCmd struct {
	conn gnet.Conn
	fun  func(gnet.Conn)
}

func NewRegistConnectionCmd(conn gnet.Conn, f func(gnet.Conn)) (cmd *RegistConnectionCmd) {
	cmd = new(RegistConnectionCmd)
	cmd.conn = conn
	cmd.fun = f

	return
}

func (r *RegistConnectionCmd) Run() {
	if r.fun != nil && r.conn != nil {
		r.fun(r.conn)
	}
}

type RegistServiceCmd struct {
	conn gnet.Conn
	msg  *protocolx.NetProtocol
	fun  func(gnet.Conn, *protocolx.NetProtocol)
}

func NewRegistServiceCmd(conn gnet.Conn, f func(gnet.Conn, *protocolx.NetProtocol), msg *protocolx.NetProtocol) (cmd *RegistServiceCmd) {
	cmd = new(RegistServiceCmd)
	cmd.conn = conn
	cmd.fun = f
	cmd.msg = msg

	return
}

func (r *RegistServiceCmd) Run() {
	if r.conn != nil && r.fun != nil && r.msg != nil {
		r.fun(r.conn, r.msg)
	}
}

type NetSessionCmd struct {
	sess gnet.NetSession
	fun  func(gnet.NetSession)
}

func NewNetSessionCmd(s gnet.NetSession, f func(gnet.NetSession)) (cmd *NetSessionCmd) {
	cmd = new(NetSessionCmd)
	cmd.fun = f
	cmd.sess = s

	return
}

func (n *NetSessionCmd) Run() {
	if n.sess != nil && n.fun != nil {
		n.fun(n.sess)
	}
}

type NormalNetCmd struct {
	sess gnet.NetSession
	msg  *protocolx.NetProtocol
	fun  gnet.NetCmdFunction
}

func NewNormalNetCmd(s gnet.NetSession, m *protocolx.NetProtocol, f gnet.NetCmdFunction) (cmd *NormalNetCmd) {
	cmd = new(NormalNetCmd)
	cmd.sess = s
	cmd.msg = m
	cmd.fun = f

	return
}

func (n *NormalNetCmd) Run() {
	if n.sess != nil && n.msg != nil && n.fun != nil {
		n.fun(n.sess, n.msg)
	}
}
