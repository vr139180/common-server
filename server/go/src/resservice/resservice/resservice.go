package resservice

import (
	"cmslib/gnet"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xsession"
	"resservice/configsvr"
	"resservice/processor"

	"google.golang.org/protobuf/proto"
)

type ResService struct {
	server.ServerBase

	//regist netcmd processor
	netProcessor *processor.NetProcessor

	//redis
	redisX *redisutil.RedisUtil

	//logins
	loginSvrs *xsession.NetSessionContainer
	homeSvrs  *xsession.NetSessionContainer

	softTimer *timerx.TimerContainer

	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool         //应用配置
	AppOption  configsvr.ResServiceConfig //本服务配置
}

//---------------------------IAppGlobal-------------------------
func (r *ResService) OnNetProcessor(s gnet.NetSession, id int, msg proto.Message) {
	r.netProcessor.OnNetProcessor(s, id, msg)
}

func (r *ResService) SendMessageToLogin(id int64, msg proto.Message) {
	r.loginSvrs.SendMessage(id, msg)
}

func (r *ResService) SendMessageToHome(id int64, msg proto.Message) {
	r.homeSvrs.SendMessage(id, msg)
}

func (r *ResService) GetRedis() *redisutil.RedisUtil {
	return r.redisX
}

func (r *ResService) GetMyNode() *eureka.ServiceNodeInfo {
	return r.eureka.GetMyNode()
}
