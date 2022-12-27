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
	"cmslib/mysqlx"
	"cmslib/protocolx"
	"cmslib/redisutil"
	"friendservice/xinf"
	"gamelib/eureka"

	"google.golang.org/protobuf/proto"
)

type IAppGlobal interface {
	PostFriendProcessor(loopIndex int, c xinf.IFriendProcessor)

	PostDBProcessor(cmd xinf.IDBProcessor)

	DoFriendsMaintance(loopIndex int)

	GetRedis() *redisutil.RedisUtil
	GetDBClient() *mysqlx.MysqlClient

	GetMyNode() *eureka.ServiceNodeInfo

	SendNetToRouter(pro *protocolx.NetProtocol)
	SendMsgToRouter(msg proto.Message)
}

var appInstance IAppGlobal

func InitAppGlobal(app IAppGlobal) {
	appInstance = app
}

func UnInitAppGlobal() {
	appInstance = nil
}

func PostFriendProcessor(loopIndex int, c xinf.IFriendProcessor) {
	appInstance.PostFriendProcessor(loopIndex, c)
}

func DoFriendsMaintance(loopIndex int) {
	appInstance.DoFriendsMaintance(loopIndex)
}

func PostDBProcessor(cmd xinf.IDBProcessor) {
	appInstance.PostDBProcessor(cmd)
}

func GetRedis() *redisutil.RedisUtil {
	return appInstance.GetRedis()
}

func GetDBClient() *mysqlx.MysqlClient {
	return appInstance.GetDBClient()
}

func GetMyNode() *eureka.ServiceNodeInfo {
	return appInstance.GetMyNode()
}

func SendNetToRouter(pro *protocolx.NetProtocol) {
	appInstance.SendNetToRouter(pro)
}

func SendMsgToRouter(msg proto.Message) {
	appInstance.SendMsgToRouter(msg)
}
