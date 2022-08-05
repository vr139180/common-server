package matchmaking

import (
	"cmslib/logx"
	"cmslib/netx"
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
	"matchmaking/processor"
)

func (l *MatchMakingService) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_MatchMaking)
	if err != nil {
		return err
	}

	return nil
}

func (l *MatchMakingService) GetLogOptions() (opts []interface{}, err error) {

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

func (l *MatchMakingService) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/matchmaking_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *MatchMakingService) InitNetwork() (err error) {
	// init factory
	l.proFactory = protobuf.NewProtobufFactory()
	// call parent initnetwork
	l.TcpSvr, err = netx.NewTCPServer(l)
	if err != nil {
		return err
	}

	subs := [...]int{int(service.ServiceType_FightRouter)}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_MatchMaking, l.configTool.Ip, l.configTool.Port, nil, subs[0:], l)

	l.fightRouterSvrs = xcluster.NewClusterServiceCtrl(l.TcpSvr, service.ServiceType_FightRouter, l)

	l.Accept(l.configTool.Ip, l.configTool.Port)

	return nil
}

func (l *MatchMakingService) InitDatabase() (err error) {

	ropt := &(l.AppOption.RedisOpt)
	l.redisX, err = redisutil.NewRedisUtil(ropt.Ip, ropt.Port, ropt.Auth, ropt.Db)
	if err != nil {
		logx.Errorf("init redis client failed")
		return err
	}

	l.softTimer = timerx.NewTimerContainer(1 * 1000)

	return nil
}

func (l *MatchMakingService) InitFinish() error {
	l.ServerBase.InitFinish()

	l.netProcessor = processor.NewMatchMakingdProcessor(l.AppOption.NetProcessorNum)
	l.netProcessor.Start()

	l.eureka.Start(l.configTool.GlobalOpt.EurekaIp, l.configTool.GlobalOpt.EurekaPort)
	l.fightRouterSvrs.Start()

	return nil
}

func (l *MatchMakingService) UnInitFinish() {

	l.fightRouterSvrs.Shutdown()
	l.netProcessor.Stop()

	l.ServerBase.UnInitFinish()
}

func (l *MatchMakingService) MainLoop() {

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
