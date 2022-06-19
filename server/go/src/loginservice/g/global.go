package g

import (
	"cmslib/gnet"
	"cmslib/mysqlx"
	"cmslib/redisutil"
	"gamelib/protobuf/gpro"

	"google.golang.org/protobuf/proto"
)

// 全局的定义
type IAppGlobal interface {
	GetDBClient() *mysqlx.MysqlClient
	GetRedis() *redisutil.RedisUtil

	RegistWaitProcess(p gnet.NetSession)
	RegistProcessNetCmd(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message)

	AskResUserSlot(p gnet.NetSession)
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

func RegistWaitProcess(p gnet.NetSession) {
	appInstance.RegistWaitProcess(p)
}

func AskResUserSlot(p gnet.NetSession) {
	appInstance.AskResUserSlot(p)
}

func RegistProcessNetCmd(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message) {
	appInstance.RegistProcessNetCmd(s, id, m)
}
