package gnet

import (
	"cmslib/gnet/pkg/errors"
	"fmt"
	"net"
	"strings"
	"time"

	"google.golang.org/protobuf/proto"
)

// SH:Lujf:SH
// 连接到其他服务器的connect,为了公用eventloop处理机制
func (s Server) ConnectTo(ip string, port int, ns NetSession) error {
	address := fmt.Sprintf("%s:%d", ip, port)

	c, err := net.Dial("tcp", address)
	if err != nil {
		return err
	}

	if _, ok := c.(*net.TCPConn); !ok {
		return errors.ErrClientOnlySupportTCP
	}

	el := s.svr.lb.next(c.RemoteAddr())
	conn := newTCPConn(c, el)
	// bind each other
	conn.SetContext(ns)
	ns.SetConn(conn)

	el.ch <- conn
	go func() {
		var buffer [0x10000]byte
		for {
			n, err := c.Read(buffer[:])
			if err != nil {
				_ = c.SetReadDeadline(time.Time{})
				el.ch <- &stderr{conn, err}
				return
			}
			el.ch <- packTCPConn(conn, buffer[:n])
		}
	}()

	return nil
}

// gnet Conn Context绑定用，用来标识连接
type NetSession interface {
	// 关联connection
	SetConn(c Conn)

	//发送消息
	SendMessage(pro proto.Message)

	// 主动关闭
	Close()

	// socket close回调
	OnClose()

	// 网络协议通知
	OnRecvMessage(id int, pro proto.Message)

	//设置附加参数
	SetExtParams(ext map[string]string)
	//获取扩展参数
	GetExtParamByKey(k string) (string, bool)
}

type NetCmdFunction = func(NetSession, int, proto.Message)

// 基础结构，实现了几个通用函数
type BaseNetSession struct {
	TcpConn Conn //连接
	msg2Fun map[int]NetCmdFunction
}

func (s *BaseNetSession) CloneExParams(e map[string]string) (c map[string]string) {
	if len(e) == 0 {
		c = nil
		return
	}

	c = make(map[string]string)
	for v, k := range e {
		c[strings.ToLower(v)] = k
	}

	return
}

func (s *BaseNetSession) SetExtParams(ext map[string]string) {
}

func (s *BaseNetSession) GetExtParamByKey(k string) (string, bool) {
	return "", false
}

// 注册协议对应的处理函数
func (s *BaseNetSession) RegistMsgMapFun(id int, fun NetCmdFunction) {
	if s.msg2Fun == nil {
		s.msg2Fun = make(map[int]NetCmdFunction)
	}

	s.msg2Fun[id] = fun
}

func (s *BaseNetSession) GetMsgMapFun(id int) (fun NetCmdFunction, err error) {
	if s.msg2Fun == nil {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	fun, ok := s.msg2Fun[id]
	if !ok {
		return nil, errors.ErrNetCmdFunNotDefined
	}

	return
}

func (s *BaseNetSession) SetConn(c Conn) {
	s.TcpConn = c
}

func (s *BaseNetSession) SendMessage(pro proto.Message) {
	if s.TcpConn == nil {
		return
	}

	s.TcpConn.AsyncWriteProtobuf(pro)
}

func (s *BaseNetSession) Close() {
	if s.TcpConn != nil {
		s.TcpConn.Close()
	}

	s.TcpConn = nil
}

// 子类重新该函数时，需要调用 BaseNetSession.OnClose()
func (s *BaseNetSession) OnClose() {
	s.TcpConn = nil
}

func (s *BaseNetSession) OnRecvMessage(id int, pro proto.Message) {
}

// SH:Lujf:SH
