package mailservice

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/netx"
	"cmslib/protocolx"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"encoding/xml"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/service"
	"gamelib/xcluster"
	"mailservice/dbprocessor"
	"mailservice/mailbox"
	"mailservice/processor"
)

func (l *MailService) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_Mail)
	if err != nil {
		return err
	}

	return nil
}

func (l *MailService) GetLogOptions() (opts []interface{}, err error) {

	opts = append(opts, logx.LoggerGlobalOption{Level: l.configTool.LogLevel})
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

func (l *MailService) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/mail_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *MailService) InitNetwork() (err error) {
	// init factory
	l.proFactory = protobuf.NewProtobufFactory()
	// call parent initnetwork
	l.TcpSvr, err = netx.NewTCPServer(l)
	if err != nil {
		return err
	}

	//chathash扩展参数
	exts := make(map[string]string)

	subs := [...]int{int(service.ServiceType_ServiceRouter)}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_Mail, l.configTool.Ip, l.configTool.Port, exts, subs[0:], l)

	var defhead protocolx.SProtocolHead
	defhead.InitHead()
	defhead.FromType = int8(service.ServiceType_Mail)
	defhead.ToType = int8(service.ServiceType_ServiceRouter)
	l.routerSvrs = xcluster.NewClusterServiceCtrl(l.TcpSvr, service.ServiceType_ServiceRouter, l, defhead)

	return nil
}

func (l *MailService) InitDatabase() (err error) {

	l.dbClient, err = mysqlx.NewMysqlClient(l.AppOption.DBOpt)
	if err != nil {
		logx.Errorf("init database client failed")
		return
	}

	ropt := &(l.AppOption.RedisOpt)
	l.redisX, err = redisutil.NewRedisUtil(ropt.Ip, ropt.Port, ropt.Auth, ropt.Db)
	if err != nil {
		logx.Errorf("init redis client failed")
		return err
	}

	l.softTimer = timerx.NewTimerContainer(1 * 1000)

	return nil
}

func (l *MailService) InitFinish() error {
	l.ServerBase.InitFinish()

	l.mbProcessor = processor.NewMailBoxProcessor(l.AppOption.MLProcessorNum)
	l.mbProcessor.Start()

	l.mailboxCtrl = mailbox.NewMailBoxCtrl(l.AppOption.MLProcessorNum)

	l.dbProcessor = dbprocessor.NewDBMailProcessor(l.AppOption.MLProcessorNum)
	l.dbProcessor.Start()

	l.eureka.Start(l.configTool.GlobalOpt.EurekaIp, l.configTool.GlobalOpt.EurekaPort)
	l.routerSvrs.Start()

	return nil
}

func (l *MailService) UnInitFinish() {

	l.mbProcessor.Stop()
	l.dbProcessor.Stop()

	l.routerSvrs.Shutdown()
	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}

func (l *MailService) MainLoop() {

	defer func() {
		l.softTimer.Stop()
	}()

	l.softTimer.Start()

	var err error

	for {
		select {
		case <-l.softTimer.Time.C:
			now := utilc.GetTimestamp()
			l.softTimer.Tick(now)
		case cmd := <-l.Ch:
			switch cmd := cmd.(type) {
			case server.ICommandBase:
				cmd.Run()
			}
		case v := <-l.Sys:
			switch x := v.(type) {
			case error:
				err = x
			}
		}

		if err == server.ErrExitApp || err == server.ErrLostEurekaExitApp {
			break
		}
	}

}
