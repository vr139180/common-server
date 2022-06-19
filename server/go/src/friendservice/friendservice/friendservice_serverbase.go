package friendservice

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/netx"
	"cmslib/redisutil"
	server "cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"encoding/xml"
	"friendservice/dbprocessor"
	"friendservice/friends"
	"friendservice/processor"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/service"
	"gamelib/xcluster"
)

func (l *FriendService) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_Friend)
	if err != nil {
		return err
	}

	return nil
}

func (l *FriendService) GetLogOptions() (opts []interface{}, err error) {

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

func (l *FriendService) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/friend_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *FriendService) InitNetwork() (err error) {
	// init factory
	l.proFactory = protobuf.NewProtobufFactory()
	// call parent initnetwork
	l.TcpSvr, err = netx.NewTCPServer(l)
	if err != nil {
		return err
	}

	//chathash扩展参数
	ext1, _ := l.configTool.GetExtParamByKey(FRIENDSERVICE_EXTPARAM_FRDHASH)
	exts := make(map[string]string)
	exts[FRIENDSERVICE_EXTPARAM_FRDHASH] = ext1

	subs := [...]int{int(service.ServiceType_Router)}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_Friend, l.configTool.Ip, l.configTool.Port, exts, subs[0:], l)

	l.routerSvrs = xcluster.NewClusterServiceCtrl(l.TcpSvr, service.ServiceType_Router, l)

	l.Accept(l.configTool.Ip, l.configTool.Port)

	return nil
}

func (l *FriendService) InitDatabase() (err error) {

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

func (l *FriendService) InitFinish() error {
	l.ServerBase.InitFinish()

	l.mbProcessor = processor.NewFriendProcessor(l.AppOption.MLProcessorNum)
	l.mbProcessor.Start()

	l.friendsCtrl = friends.NewFriendsCtrl(l.AppOption.MLProcessorNum)

	l.dbProcessor = dbprocessor.NewDBFriendProcessor(l.AppOption.MLProcessorNum)
	l.dbProcessor.Start()

	l.eureka.Start(l.configTool.GlobalOpt.EurekaIp, l.configTool.GlobalOpt.EurekaPort)
	l.routerSvrs.Start()

	return nil
}

func (l *FriendService) UnInitFinish() {

	l.mbProcessor.Stop()
	l.dbProcessor.Stop()

	l.routerSvrs.Shutdown()
	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}

func (l *FriendService) MainLoop() {

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
