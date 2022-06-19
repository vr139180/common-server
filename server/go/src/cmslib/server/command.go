package server

import (
	"cmslib/gnet"

	"google.golang.org/protobuf/proto"
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
	msg  proto.Message
	fun  func(gnet.Conn, proto.Message)
}

func NewRegistServiceCmd(conn gnet.Conn, f func(gnet.Conn, proto.Message), msg proto.Message) (cmd *RegistServiceCmd) {
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
	id   int
	msg  proto.Message
	fun  gnet.NetCmdFunction
}

func NewNormalNetCmd(s gnet.NetSession, id int, m proto.Message, f gnet.NetCmdFunction) (cmd *NormalNetCmd) {
	cmd = new(NormalNetCmd)
	cmd.sess = s
	cmd.id = id
	cmd.msg = m
	cmd.fun = f

	return
}

func (n *NormalNetCmd) Run() {
	if n.sess != nil && n.msg != nil && n.fun != nil {
		n.fun(n.sess, n.id, n.msg)
	}
}
