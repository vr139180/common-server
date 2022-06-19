package chatservice

import (
	"chatservice/channel"
	"chatservice/configsvr"
	"chatservice/processor"
	"chatservice/xinf"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xcluster"

	"google.golang.org/protobuf/proto"
)

type ChatService struct {
	server.ServerBase

	//regist netcmd processor
	chProcessor *processor.ChannelProcessor
	channelCtrl *channel.ChannelCtrl

	//redis
	redisX *redisutil.RedisUtil

	routerSvrs *xcluster.ClusterServiceCtrl

	softTimer *timerx.TimerContainer

	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool          //应用配置
	AppOption  configsvr.ChatServiceConfig //本服务配置
}

//---------------------------IAppGlobal-------------------------
func (r *ChatService) PostChatProcessor(loopIndex int, c xinf.IChatProcessor) {
	r.chProcessor.PostChatToProcessor(loopIndex, c)
}

func (r *ChatService) GetRedis() *redisutil.RedisUtil {
	return r.redisX
}

func (r *ChatService) SendMsgToRouter(msg proto.Message) {
	r.routerSvrs.SendMessage(msg)
}

func (r *ChatService) DoChannelMaintance(loopIndex int) {
	r.channelCtrl.DoChannelMaintance(loopIndex)
}
