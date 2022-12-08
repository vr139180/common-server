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

package g

import (
	"chatservice/xinf"
	"cmslib/protocolx"
	"cmslib/redisutil"
	"gamelib/eureka"
)

type IAppGlobal interface {
	PostChatProcessor(loopIndex int, c xinf.IChatProcessor)

	DoChannelMaintance(loopIndex int)

	GetRedis() *redisutil.RedisUtil

	GetMyNode() *eureka.ServiceNodeInfo

	SendMsgToRouter(pro *protocolx.NetProtocol)
}

var appInstance IAppGlobal

func InitAppGlobal(app IAppGlobal) {
	appInstance = app
}

func UnInitAppGlobal() {
	appInstance = nil
}

func PostChatProcessor(loopIndex int, c xinf.IChatProcessor) {
	appInstance.PostChatProcessor(loopIndex, c)
}

func DoChannelMaintance(loopIndex int) {
	appInstance.DoChannelMaintance(loopIndex)
}

func GetRedis() *redisutil.RedisUtil {
	return appInstance.GetRedis()
}

func GetMyNode() *eureka.ServiceNodeInfo {
	return appInstance.GetMyNode()
}

func SendMsgToRouter(pro *protocolx.NetProtocol) {
	appInstance.SendMsgToRouter(pro)
}
