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
