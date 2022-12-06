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

package datasync

import (
	"cmslib/mysqlx"
	"cmslib/redisutil"
	server "cmslib/server"
	"datasync/configsvr"
	"datasync/dbserialize"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
)

type DataSync struct {
	server.ServerBase

	serializeTool *dbserialize.DBSerializePool

	//mysql instance
	dbClient *mysqlx.MysqlClient
	redisX   *redisutil.RedisUtil

	//-------------------------------------------------
	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool       //应用配置
	AppOption  configsvr.DataSyncConfig //本服务配置
}

//-------------------------------------IAppGlobal------------------------------------
func (ls *DataSync) GetDBClient() *mysqlx.MysqlClient {
	return ls.dbClient
}

func (ls *DataSync) GetRedis() *redisutil.RedisUtil {
	return ls.redisX
}
