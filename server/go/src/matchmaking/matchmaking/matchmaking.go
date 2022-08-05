package matchmaking

import (
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xcluster"
	"matchmaking/configsvr"
	"matchmaking/processor"

	"google.golang.org/protobuf/proto"
)

type MatchMakingService struct {
	server.ServerBase

	//regist netcmd processor
	netProcessor *processor.MatchMakingProcessor

	//redis
	redisX *redisutil.RedisUtil

	fightRouterSvrs *xcluster.ClusterServiceCtrl

	softTimer *timerx.TimerContainer

	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool          //应用配置
	AppOption  configsvr.MatchMakingConfig //本服务配置
}

//---------------------------IAppGlobal-------------------------
func (r *MatchMakingService) SendMsgToRouter(msg proto.Message) {
	r.fightRouterSvrs.SendMessage(msg)
}

func (r *MatchMakingService) GetRedis() *redisutil.RedisUtil {
	return r.redisX
}

func (r *MatchMakingService) GetMyNode() *eureka.ServiceNodeInfo {
	return r.eureka.GetMyNode()
}
