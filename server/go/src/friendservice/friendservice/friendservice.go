package friendservice

import (
	"cmslib/mysqlx"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"friendservice/configsvr"
	"friendservice/dbprocessor"
	"friendservice/friends"
	"friendservice/processor"
	"friendservice/xinf"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xcluster"

	"google.golang.org/protobuf/proto"
)

type FriendService struct {
	server.ServerBase

	//regist netcmd processor
	mbProcessor *processor.FriendProcessor
	friendsCtrl *friends.FriendsCtrl

	dbProcessor *dbprocessor.DBFriendProcessor

	//redis
	redisX   *redisutil.RedisUtil
	dbClient *mysqlx.MysqlClient

	routerSvrs *xcluster.ClusterServiceCtrl

	softTimer *timerx.TimerContainer

	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool            //应用配置
	AppOption  configsvr.FriendServiceConfig //本服务配置
}

//---------------------------IAppGlobal-------------------------
func (r *FriendService) PostFriendProcessor(loopIndex int, c xinf.IFriendProcessor) {
	r.mbProcessor.PostFriendToProcessor(loopIndex, c)
}

func (r *FriendService) GetRedis() *redisutil.RedisUtil {
	return r.redisX
}

func (r *FriendService) GetDBClient() *mysqlx.MysqlClient {
	return r.dbClient
}

func (r *FriendService) SendMsgToRouter(msg proto.Message) {
	r.routerSvrs.SendMessage(msg)
}

func (r *FriendService) DoFriendsMaintance(loopIndex int) {
	r.friendsCtrl.DoFriendsMaintance(loopIndex)
}

func (r *FriendService) PostDBProcessor(cmd xinf.IDBProcessor) {
	r.dbProcessor.PostDBProcessor(cmd)
}
