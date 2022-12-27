// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package friendservice

import (
	"cmslib/mysqlx"
	"cmslib/protocolx"
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
	"gamelib/service"
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

func (r *FriendService) SendNetToRouter(pro *protocolx.NetProtocol) {
	r.routerSvrs.SendNetMessage2(pro)
}

func (r *FriendService) SendMsgToRouter(msg proto.Message) {
	pro := protocolx.NewNetProtocolByMsg(msg)

	head := pro.WriteHead()
	head.ToType = int8(service.ServiceType_ServiceRouter)
	head.FromType = int8(service.ServiceType_Friend)
	r.routerSvrs.SendNetMessage2(pro)
}

func (r *FriendService) DoFriendsMaintance(loopIndex int) {
	r.friendsCtrl.DoFriendsMaintance(loopIndex)
}

func (r *FriendService) PostDBProcessor(cmd xinf.IDBProcessor) {
	r.dbProcessor.PostDBProcessor(cmd)
}
