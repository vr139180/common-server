package loginservice

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/netx"
	"cmslib/redisutil"
	"cmslib/utilc"
	"encoding/xml"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/service"
	"gamelib/xcluster"
	"loginservice/registration"
	"loginservice/verification"
)

func (l *LoginService) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_Login)
	if err != nil {
		return err
	}

	return nil
}

func (l *LoginService) GetLogOptions() (opts []interface{}, err error) {

	opts = append(opts,
		logx.LoggerGlobalOption{
			Level: l.configTool.LogLevel})

	opts = append(opts, logx.ConsoleLogOption{})

	bpath := l.configTool.LogDir
	if len(bpath) == 0 {
		bpath, err = utilc.GetBinPath()
		if err != nil {
			return nil, err
		}
	}

	opts = append(opts,
		logx.FileRotateLogOption{
			BasePath:      bpath,
			FileName:      l.configTool.LogFile,
			RemainFileNum: 7})

	return opts, nil
}

func (l *LoginService) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/login_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *LoginService) InitNetwork() (err error) {
	// init factory
	l.proFactory = protobuf.NewProtobufFactory()
	// call parent initnetwork
	l.TcpSvr, err = netx.NewTCPServer(l)
	if err != nil {
		return err
	}

	subs := [...]int{int(service.ServiceType_Res)}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_Login, l.configTool.Ip, l.configTool.Port, nil, subs[0:], l)

	l.resCtrl = xcluster.NewClusterServiceCtrl(l.TcpSvr, service.ServiceType_Res, l)

	l.Accept(l.configTool.Ip, l.configTool.Port)

	l.verifyTools = verification.NewVerifyControl(l.AppOption.VerifyLoopNum)
	l.registTools = registration.NewRegistControl(l.AppOption.RegistLoopNum)

	return nil
}

func (l *LoginService) InitDatabase() (err error) {
	l.dbClient, err = mysqlx.NewMysqlClient(l.AppOption.DBOpt)
	if err != nil {
		return
	}

	ropt := &(l.AppOption.RedisOpt)
	l.redisX, err = redisutil.NewRedisUtil(ropt.Ip, ropt.Port, ropt.Auth, ropt.Db)
	if err != nil {
		logx.Errorf("init redis client failed")
		return err
	}

	return nil
}

func (l *LoginService) InitFinish() error {
	l.ServerBase.InitFinish()

	l.eureka.Start(l.configTool.GlobalOpt.EurekaIp, l.configTool.GlobalOpt.EurekaPort)
	l.resCtrl.Start()

	l.verifyTools.Start()
	l.registTools.Start()

	return nil
}

func (l *LoginService) UnInitFinish() {

	l.verifyTools.Stop()
	l.registTools.Stop()

	l.resCtrl.Shutdown()
	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}
