package g

import (
	"chatservice/xinf"
	"cmslib/redisutil"
	"gamelib/eureka"

	"google.golang.org/protobuf/proto"
)

type IAppGlobal interface {
	PostChatProcessor(loopIndex int, c xinf.IChatProcessor)

	DoChannelMaintance(loopIndex int)

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

func PostChatProcessor(loopIndex int, c xinf.IChatProcessor) {
	appInstance.PostChatProcessor(loopIndex, c)
}

func DoChannelMaintance(loopIndex int) {
	appInstance.DoChannelMaintance(loopIndex)
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
