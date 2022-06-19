package datasync

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/netx"
	"cmslib/redisutil"
	"cmslib/utilc"
	"datasync/dbserialize"
	"encoding/xml"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/service"
)

func (l *DataSync) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_DataSync)
	if err != nil {
		return err
	}

	return nil
}

func (l *DataSync) GetLogOptions() (opts []interface{}, err error) {

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

func (l *DataSync) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/datasync_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *DataSync) InitNetwork() (err error) {
	// init factory
	l.proFactory = protobuf.NewProtobufFactory()
	// call parent initnetwork
	l.TcpSvr, err = netx.NewTCPServer(l)
	if err != nil {
		return err
	}

	subs := [...]int{}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_DataSync, l.configTool.Ip, l.configTool.Port, nil, subs[0:], l)

	l.Accept(l.configTool.Ip, l.configTool.Port)

	l.serializeTool = dbserialize.NewDBSerializePool(l.AppOption.SerializeLoopNum)

	return nil
}

func (l *DataSync) InitDatabase() (err error) {
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

func (l *DataSync) InitFinish() error {
	l.ServerBase.InitFinish()

	l.eureka.Start(l.configTool.GlobalOpt.EurekaIp, l.configTool.GlobalOpt.EurekaPort)

	l.serializeTool.Start()

	return nil
}

func (l *DataSync) UnInitFinish() {

	l.serializeTool.Stop()

	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}
