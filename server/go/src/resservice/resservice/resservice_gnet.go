package resservice

import (
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/server"
	"errors"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"gamelib/service"
	"resservice/net"

	"google.golang.org/protobuf/proto"
)

func (l *ResService) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *ResService) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("Res Service is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *ResService) OnTCPInitComplete() {
	logx.Warnf("<<<<<<<<<<<<Res Service listen at %s:%d>>>>>>>>>>>>", l.configTool.Ip, l.configTool.Port)
}

func (l *ResService) OnTCPShutdown() {
}

func (l *ResService) OnTCPOpened(c gnet.Conn) {
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
		if !l.eureka.IsReady() {
			c.Close()
			return
		}

		cmd := server.NewRegistConnectionCmd(c, l.onRecvNewConnection)
		l.RegistCommand(cmd)
	}
}

func (l *ResService) OnTCPClosed(c gnet.Conn, err error) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeDisConnected(s)
			case *net.LoginNetSession:
				l.loginSvrs.DeleteOneSession(s.SIid)
			case *net.HomeNetSession:
				l.homeSvrs.DeleteOneSession(s.SIid)
			default:
			}
		}
	}
}

func (l *ResService) OnRecvMessage(c gnet.Conn, id int, m proto.Message) {
	s := c.Context()
	if s != nil {
		ns, ok := s.(gnet.NetSession)
		if ok {
			ns.OnRecvMessage(id, m)
		}
	} else {
		if id == int(gpro.ERK_PROTYPE_SVR_SERVICEBINDSERVICE_REQ) {
			r, _ := m.(*gpro.Svr_ServiceBindServiceReq)
			if r.SvrType == int32(service.ServiceType_Login) {
				//login的注册
				ns := net.NewLoginNetSession(c, r.GetMyiid())
				l.loginSvrs.RegistNetSession(r, ns)
			} else if r.SvrType == int32(service.ServiceType_Home) {
				//login的注册
				ns := net.NewHomeNetSession(c, r.GetMyiid())
				l.homeSvrs.RegistNetSession(r, ns)
			}
		}
	}
}

func (l *ResService) onRecvNewConnection(conn gnet.Conn) {
}
