package g

import (
	"cmslib/mysqlx"
	"cmslib/redisutil"
)

// 全局的定义
type IAppGlobal interface {
	GetDBClient() *mysqlx.MysqlClient
	GetRedis() *redisutil.RedisUtil
}

var appInstance IAppGlobal

func InitAppGlobal(app IAppGlobal) {
	appInstance = app
}

func UnInitAppGlobal() {
	appInstance = nil
}

func DBS() *mysqlx.MysqlClient {
	return appInstance.GetDBClient()
}

func GetRedis() *redisutil.RedisUtil {
	return appInstance.GetRedis()
}
