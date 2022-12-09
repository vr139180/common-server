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
	"gamelib/eureka"
	"mailservice/xinf"
)

type IAppGlobal interface {
	PostMailProcessor(loopIndex int, c xinf.IMailProcessor)
	PostToSysMailProcessor(c xinf.IMailProcessor)

	PostDBProcessor(cmd xinf.IDBProcessor)

	DoMailBoxMaintance(loopIndex int)

	GetRedis() *redisutil.RedisUtil
	GetDBClient() *mysqlx.MysqlClient

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

func PostMailProcessor(loopIndex int, c xinf.IMailProcessor) {
	appInstance.PostMailProcessor(loopIndex, c)
}

func PostToSysMailProcessor(c xinf.IMailProcessor) {
	appInstance.PostToSysMailProcessor(c)
}

func DoMailBoxMaintance(loopIndex int) {
	appInstance.DoMailBoxMaintance(loopIndex)
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

func SendMsgToRouter(pro *protocolx.NetProtocol) {
	appInstance.SendMsgToRouter(pro)
}
