package g

import (
	"cmslib/mysqlx"
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

func SendMsgToRouter(msg proto.Message) {
	appInstance.SendMsgToRouter(msg)
}
