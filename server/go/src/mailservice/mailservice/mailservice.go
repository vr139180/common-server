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

	"google.golang.org/protobuf/proto"
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

func (r *MailService) SendMsgToRouter(totype int8, token protocolx.UserToken, msg proto.Message) {
	r.routerSvrs.SendNetMessage(totype, token, msg)
}

func (r *MailService) DoMailBoxMaintance(loopIndex int) {
	r.mailboxCtrl.DoMailBoxMaintance(loopIndex)
}

func (r *MailService) PostDBProcessor(cmd xinf.IDBProcessor) {
	r.dbProcessor.PostDBProcessor(cmd)
}
