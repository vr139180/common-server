package g

import (
	"cmslib/gnet"
	"cmslib/redisutil"
	"gamelib/eureka"

	"google.golang.org/protobuf/proto"
)

type IAppGlobal interface {
	OnNetProcessor(s gnet.NetSession, id int, msg proto.Message)
	SendMessageToLogin(id int64, msg proto.Message)
	SendMessageToHome(id int64, msg proto.Message)

	GetRedis() *redisutil.RedisUtil

	GetMyNode() *eureka.ServiceNodeInfo
}

var appInstance IAppGlobal

func InitAppGlobal(app IAppGlobal) {
	appInstance = app
}

func UnInitAppGlobal() {
	appInstance = nil
}

func OnNetProcessor(s gnet.NetSession, id int, msg proto.Message) {
	appInstance.OnNetProcessor(s, id, msg)
}

func SendMessageToLogin(id int64, msg proto.Message) {
	appInstance.SendMessageToLogin(id, msg)
}

func SendMessageToHome(id int64, msg proto.Message) {
	appInstance.SendMessageToHome(id, msg)
}

func GetRedis() *redisutil.RedisUtil {
	return appInstance.GetRedis()
}

func GetMyNode() *eureka.ServiceNodeInfo {
	return appInstance.GetMyNode()
}
