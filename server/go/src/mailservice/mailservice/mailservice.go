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

package mailservice

import (
	"cmslib/mysqlx"
	"cmslib/protocolx"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/xcluster"
	"mailservice/configsvr"
	"mailservice/dbprocessor"
	"mailservice/mailbox"
	"mailservice/processor"
	"mailservice/xinf"
)

type MailService struct {
	server.ServerBase

	//regist netcmd processor
	mbProcessor *processor.MailBoxProcessor
	mailboxCtrl *mailbox.MailBoxCtrl

	dbProcessor *dbprocessor.DBMailProcessor

	//redis
	redisX   *redisutil.RedisUtil
	dbClient *mysqlx.MysqlClient

	routerSvrs *xcluster.ClusterServiceCtrl

	softTimer *timerx.TimerContainer

	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool          //应用配置
	AppOption  configsvr.MailServiceConfig //本服务配置
}

//---------------------------IAppGlobal-------------------------
func (r *MailService) PostMailProcessor(loopIndex int, c xinf.IMailProcessor) {
	r.mbProcessor.PostMailToProcessor(loopIndex, c)
}

func (r *MailService) PostToSysMailProcessor(c xinf.IMailProcessor) {
	r.mbProcessor.PostToSysMailProcessor(c)
}

func (r *MailService) GetRedis() *redisutil.RedisUtil {
	return r.redisX
}

func (r *MailService) GetDBClient() *mysqlx.MysqlClient {
	return r.dbClient
}

func (r *MailService) SendMsgToRouter(pro *protocolx.NetProtocol) {
	r.routerSvrs.SendNetMessage2(pro)
}

func (r *MailService) DoMailBoxMaintance(loopIndex int) {
	r.mailboxCtrl.DoMailBoxMaintance(loopIndex)
}

func (r *MailService) PostDBProcessor(cmd xinf.IDBProcessor) {
	r.dbProcessor.PostDBProcessor(cmd)
}
