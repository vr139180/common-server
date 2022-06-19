package loginservice

import (
	"cmslib/gnet"
	"cmslib/logx"
	"errors"
	"gamelib/eureka"
	"gamelib/protobuf/gpro"
	"loginservice/microsvr"
	"loginservice/processor"

	"google.golang.org/protobuf/proto"
)

func (l *LoginService) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *LoginService) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("LoginService is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *LoginService) OnTCPInitComplete() {
	logx.Infof("LoginService is listening on %s:%d", l.configTool.Ip, l.configTool.Port)
}

func (l *LoginService) OnTCPShutdown() {
}

func (l *LoginService) OnTCPOpened(c gnet.Conn) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeConnected(s)
			case *microsvr.ResNetSession:
				l.resCtrl.OnResNodeConnected(s)
			default:
			}
		} else {
			panic(errors.New("not support other interface, only gnet.NetSession"))
		}
	} else if !l.eureka.IsReady() {
		logx.Infof("login service recv client connection request, but service is not ready.")
		c.Close()
	} else {
		//add to wait list
		l.verifyTools.RegistNewSession(c)
	}
	//when accept a new connection, dont process it, util recv the login message.
}

func (l *LoginService) OnTCPClosed(c gnet.Conn, err error) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeDisConnected(s)
			case *microsvr.ResNetSession:
				l.resCtrl.OnResNodeDisConnected(s)
			case *processor.LoginProcessor:
				s.OnClose()
			default:
			}
		}
	} else {
		l.verifyTools.RemoveNetSession(c)
	}
}

func (l *LoginService) OnRecvMessage(c gnet.Conn, id int, m proto.Message) {
	s := c.Context()
	if s != nil {
		ns, ok := s.(gnet.NetSession)
		if ok {
			ns.OnRecvMessage(id, m)
		}
	} else {
		if !l.eureka.IsReady() {
			c.Close()
			return
		}

		//没有关联netsession, 收到登录协议时开始绑定
		//客户端连接之后是否马上发送登录协议，不处理
		if id == int(gpro.USER_PROTYPE_USER_LOGIN_REQ) {
			r, _ := m.(*gpro.User_LoginReq)
			l.verifyTools.NewTcpConnection(c, int(r.Type), r.GetUserid(), r.GetToken(), r.GetAccount(), r.GetPwd())
		}
	}
}
