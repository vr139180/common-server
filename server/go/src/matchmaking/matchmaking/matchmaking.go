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
