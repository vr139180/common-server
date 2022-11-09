package g

import (
	"cmslib/mysqlx"
	"cmslib/protocolx"
	"cmslib/redisutil"
	"gamelib/eureka"
	"mailservice/xinf"

	"google.golang.org/protobuf/proto"
)

type IAppGlobal interface {
	PostMailProcessor(loopIndex int, c xinf.IMailProcessor)
	PostToSysMailProcessor(c xinf.IMailProcessor)

	PostDBProcessor(cmd xinf.IDBProcessor)

	DoMailBoxMaintance(loopIndex int)

	GetRedis() *redisutil.RedisUtil
	GetDBClient() *mysqlx.MysqlClient

	GetMyNode() *eureka.ServiceNodeInfo

	SendMsgToRouter(totype int8, token protocolx.UserToken, msg proto.Message)
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

func SendMsgToRouter(totype int8, token protocolx.UserToken, msg proto.Message) {
	appInstance.SendMsgToRouter(totype, token, msg)
}
