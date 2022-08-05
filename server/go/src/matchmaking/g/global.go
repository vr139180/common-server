package g

import (
	"cmslib/redisutil"
	"gamelib/eureka"

	"google.golang.org/protobuf/proto"
)

type IAppGlobal interface {
	GetRedis() *redisutil.RedisUtil

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

func GetRedis() *redisutil.RedisUtil {
	return appInstance.GetRedis()
}

func GetMyNode() *eureka.ServiceNodeInfo {
	return appInstance.GetMyNode()
}

func SendMsgToRouter(msg proto.Message) {
	appInstance.SendMsgToRouter(msg)
}
