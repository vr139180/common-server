// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

package datasync

import (
	"cmslib/logx"
	"cmslib/mysqlx"
	"cmslib/netx"
	"cmslib/redisutil"
	"cmslib/utilc"
	"datasync/dbserialize"
	"encoding/xml"
	"errors"
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

	ok, node := eureka.GetEurekaMasterInfo(l.configTool.GlobalOpt.Eureka)
	if !ok {
		return errors.New("get eureka master node failed.")
	}

	subs := [...]int{}
	balances := [...]int{}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_DataSync, l.configTool.Ip, l.configTool.Port, nil,
		node, subs[0:], balances[0:], false, l)

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
	l.redisX, err = redisutil.NewRedisUtil(ropt)
	if err != nil {
		logx.Errorf("init redis client failed")
		return err
	}

	return nil
}

func (l *DataSync) InitFinish() error {
	l.ServerBase.InitFinish()

	l.eureka.Start()

	l.serializeTool.Start()

	return nil
}

func (l *DataSync) UnInitFinish() {

	l.serializeTool.Stop()

	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}
