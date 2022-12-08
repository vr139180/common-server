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


package chatservice

import (
	"chatservice/channel"
	"chatservice/processor"
	"cmslib/logx"
	"cmslib/netx"
	server "cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"encoding/xml"
	"errors"
	"gamelib/config"
	"gamelib/eureka"
	"gamelib/protobuf"
	"gamelib/service"
	"gamelib/xcluster"
)

func (l *ChatService) LoadLocalConfig() (err error) {
	l.configTool, err = config.NewConfig(service.ServiceType_Chat)
	if err != nil {
		return err
	}

	return nil
}

func (l *ChatService) GetLogOptions() (opts []interface{}, err error) {

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

func (l *ChatService) LoadGameConfig() (err error) {

	err = l.configTool.LoadGlobalConfig()
	if err != nil {
		return err
	}

	cstr, err := l.configTool.LoadConfigContent("/system/chat_config.xml")
	if err != nil {
		return err
	}

	err = xml.Unmarshal(cstr, &l.AppOption)
	if err != nil {
		return err
	}

	return nil
}

func (l *ChatService) InitNetwork() (err error) {
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

	subs := [...]int{int(service.ServiceType_ServiceRouter)}
	balances := [...]int{int(service.ServiceType_Chat)}
	l.eureka = eureka.NewEurekaCluster(l.TcpSvr, service.ServiceType_Chat, l.configTool.Ip, l.configTool.Port, nil,
		node, subs[0:], balances[0:], false, l)

	l.routerSvrs = xcluster.NewClusterServiceCtrl(l.TcpSvr, service.ServiceType_ServiceRouter, l)

	l.Accept(l.configTool.Ip, l.configTool.Port, true)

	return nil
}

func (l *ChatService) InitDatabase() (err error) {

	ropt := &(l.AppOption.RedisOpt)
	l.redisX, err = redisutil.NewRedisUtil(ropt)
	if err != nil {
		logx.Errorf("init redis client failed")
		return err
	}

	l.softTimer = timerx.NewTimerContainer(1 * 1000)

	return nil
}

func (l *ChatService) InitFinish() error {
	l.ServerBase.InitFinish()

	l.chProcessor = processor.NewChannelProcessor(l.AppOption.CHProcessorNum)
	l.chProcessor.Start()

	l.channelCtrl = channel.NewChannelCtrl(l.AppOption.CHProcessorNum)

	l.eureka.Start()
	
	l.routerSvrs.Start()

	return nil
}

func (l *ChatService) UnInitFinish() {

	l.chProcessor.Stop()

	l.routerSvrs.Shutdown()
	l.eureka.Shutdown()

	l.ServerBase.UnInitFinish()
}

func (l *ChatService) MainLoop() {

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
