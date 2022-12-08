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

package chatservice

import (
	"chatservice/channel"
	"chatservice/configsvr"
	"chatservice/processor"
	"chatservice/xinf"
	"cmslib/protocolx"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xcluster"
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

func (r *ChatService) SendMsgToRouter(pro *protocolx.NetProtocol) {
	r.routerSvrs.SendNetMessage2(pro)
}

func (r *ChatService) DoChannelMaintance(loopIndex int) {
	r.channelCtrl.DoChannelMaintance(loopIndex)
}
