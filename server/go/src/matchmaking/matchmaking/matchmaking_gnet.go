package matchmaking

import (
	"cmslib/gnet"
	"cmslib/logx"
	"errors"
	"gamelib/eureka"
	"matchmaking/net"

	"google.golang.org/protobuf/proto"
)

func (l *MatchMakingService) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	fact = l.proFactory
	return
}

func (l *MatchMakingService) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	logx.Infof("Matchmaking Service is listening on %s (multi-cores: %t, loops: %d)",
		svr.Addr.String(), svr.Multicore, svr.NumEventLoop)

	return
}

func (l *MatchMakingService) OnTCPInitComplete() {
	logx.Warnf("<<<<<<<<<<<<Matchmaking Service listen at %s:%d>>>>>>>>>>>>", l.configTool.Ip, l.configTool.Port)
}

func (l *MatchMakingService) OnTCPShutdown() {
}

func (l *MatchMakingService) OnTCPOpened(c gnet.Conn) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeConnected(s)
			case *net.FightRouterNetSession:
				l.fightRouterSvrs.OnResNodeConnected(s)
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

func (l *MatchMakingService) OnTCPClosed(c gnet.Conn, err error) {
	ns := c.Context()
	if ns != nil {
		n, ok := ns.(gnet.NetSession)
		if ok {
			switch s := n.(type) {
			case *eureka.EurekaSession:
				// 通知有节点连接成功
				l.eureka.OnEurekaNodeDisConnected(s)
			case *net.FightRouterNetSession:
				l.fightRouterSvrs.OnResNodeDisConnected(s)
			default:
			}
		}
	}
}

func (l *MatchMakingService) OnRecvMessage(c gnet.Conn, id int, m proto.Message) {
	s := c.Context()
	if s != nil {
		ns, ok := s.(gnet.NetSession)
		if ok {
			ns.OnRecvMessage(id, m)
		}
	}
}
