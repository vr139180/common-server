package loginservice

import (
	"cmslib/gnet"
	"cmslib/mysqlx"
	"cmslib/redisutil"
	server "cmslib/server"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/protobuf/gpro"
	"gamelib/xcluster"
	"loginservice/configsvr"
	"loginservice/processor"
	"loginservice/registration"
	"loginservice/verification"

	"google.golang.org/protobuf/proto"
)

type LoginService struct {
	server.ServerBase

	verifyTools *verification.VerifyControl
	registTools *registration.RegistControl

	//mysql instance
	dbClient *mysqlx.MysqlClient
	redisX   *redisutil.RedisUtil

	//res资源分配服务
	resCtrl *xcluster.ClusterServiceCtrl

	//-------------------------------------------------
	eureka     *eureka.EurekaCluster // 注册中心
	proFactory *protobuf.ProtobufFactory
	configTool *config.ConfigTool           //应用配置
	AppOption  configsvr.LoginServiceConfig //本服务配置
}

//-------------------------------------IAppGlobal------------------------------------
func (ls *LoginService) GetDBClient() *mysqlx.MysqlClient {
	return ls.dbClient
}

func (ls *LoginService) RegistWaitProcess(p gnet.NetSession) {
	lp := p.(*processor.LoginProcessor)
	ls.registTools.RegistWaitProcess(lp)
}

func (ls *LoginService) AskResUserSlot(p gnet.NetSession) {
	lp := p.(*processor.LoginProcessor)
	req := lp.AskLoginAsset()
	//填充lsno
	req.Lsno = ls.eureka.GetMyNode().Iid

	ls.resCtrl.SendMessage(req)
}

func (ls *LoginService) RegistProcessNetCmd(s gnet.NetSession, id gpro.ERK_PROTYPE, m proto.Message) {
	ls.registTools.ProcessNetCmd(s, id, m)
}

func (ls *LoginService) GetRedis() *redisutil.RedisUtil {
	return ls.redisX
}
