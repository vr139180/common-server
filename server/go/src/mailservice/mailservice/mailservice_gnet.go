package mailservice

import (
	"cmslib/gnet"
	"cmslib/logx"
	"errors"
	"gamelib/eureka"
	"mailservice/net"

	"google.golang.org/protobuf/proto"
)

func (l *MailService) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *MailService) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("Mail Service is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *MailService) OnTCPInitComplete() {
	logx.Warnf("<<<<<<<<<<<<MailService listen at %s:%d>>>>>>>>>>>>", l.configTool.Ip, l.configTool.Port)
}

func (l *MailService) OnTCPShutdown() {
}

func (l *MailService) OnTCPOpened(c gnet.Conn) {
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
		if !l.eureka.IsReady() {
			c.Close()
			return
		}
	}
}

func (l *MailService) OnTCPClosed(c gnet.Conn, err error) {
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

func (l *MailService) OnRecvMessage(c gnet.Conn, id int, m proto.Message) {
	s := c.Context()
	if s != nil {
		ns, ok := s.(gnet.NetSession)
		if ok {
			ns.OnRecvMessage(id, m)
		}
	}
}
