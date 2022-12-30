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

package server

import (
	"cmslib/gnet"
	"cmslib/logx"
	"cmslib/netx"
	"cmslib/protocolx"
	"errors"
	"runtime"
)

// 服务app接口
type IServerApp interface {
	GetLogOptions() ([]interface{}, error)
	LoadLocalConfig() error
	LoadGameConfig() error
	InitNetwork() error
	InitDatabase() error
	RegisterTimer()
	InitFinish() error
	MainLoop()
	UnInitDatabase()
	UnInitNetwork()
	UnInitFinish()
}

const CmdPoolMaxDefault int = 1000

var ErrExitApp = errors.New("try to quit application")
var ErrLostEurekaExitApp = errors.New("application lost all eureka connections. will quit application")

// 服务基础类
type ServerBase struct {
	TcpSvr     *netx.TCPServer  //tcpserver
	Ch         chan interface{} //command channel
	Sys        chan interface{} //exit channel
	CmdPoolMax int              //command channel max pool
}

func (s *ServerBase) LoadLocalConfig() error {
	return errors.New("must implement ServerBase.LoadLocalConfig function")
}

func (s *ServerBase) GetLogOptions() ([]interface{}, error) {
	return nil, errors.New("must implement ServerBase.InitLog function")
}

func (s *ServerBase) LoadGameConfig() error {
	return errors.New("must implement ServerBase.LoadGameConfig function")
}

func (s *ServerBase) InitNetwork() (err error) {
	return errors.New("must call Server.InitNetwork function")
}

func (s *ServerBase) UnInitNetwork() {
}

func (s *ServerBase) InitDatabase() error {
	return errors.New("must implement ServerBase.InitDatabase function")
}

func (s *ServerBase) UnInitDatabase() {

}

func (s *ServerBase) RegisterTimer() {
}

func (s *ServerBase) InitFinish() error {
	if s.CmdPoolMax == 0 {
		s.CmdPoolMax = CmdPoolMaxDefault
	}

	s.Ch = make(chan interface{}, s.CmdPoolMax)
	s.Sys = make(chan interface{}, 1)

	return nil
}

func (s *ServerBase) UnInitFinish() {

}

func (s *ServerBase) ExitApp() {
	s.Sys <- ErrExitApp
	close(s.Sys)
}

func (s *ServerBase) ExitAppOfLostEureka() {
	s.Sys <- ErrLostEurekaExitApp
	close(s.Sys)
}

func (s *ServerBase) MainLoop() {
	var err error
	for {
		select {
		case cmd := <-s.Ch:
			switch cmd := cmd.(type) {
			case ICommandBase:
				cmd.Run()
			}

		case v := <-s.Sys:
			switch x := v.(type) {
			case error:
				err = x
			}
		}

		if err == ErrExitApp || err == ErrLostEurekaExitApp {
			break
		}
	}
}

//--------------ITCPServerApp implementation------------------
func (s *ServerBase) GetProtoFactory() (fact gnet.IProtobufFactory, err error) {
	err = errors.New("must implement ServerBase.GetProtoFactory function")
	return
}

func (s *ServerBase) OnTCPInitComplete() {
}
func (s *ServerBase) OnTCPShutdown() {
}
func (s *ServerBase) OnTCPOpened(c gnet.Conn) {
}
func (s *ServerBase) OnTCPClosed(c gnet.Conn, err error) {
}

func (s *ServerBase) OnRecvMessage(c gnet.Conn, m *protocolx.NetProtocol) {
}

//-----------------------------------------------------------

// listen on tcp ip,port, true: dont listen
func (s *ServerBase) Accept(ip string, port int) {
	go s.TcpSvr.Accept(ip, port)
}

func (s *ServerBase) RegistCommand(cmd ICommandBase) {
	s.Ch <- cmd
}

// 启动类基础封装
type ServerWrapper struct {
	svr IServerApp
}

func NewServerWrapper(s IServerApp) *ServerWrapper {
	sw := ServerWrapper{s}
	return &sw
}

func (s ServerWrapper) initLogger(opt ...interface{}) error {
	err := logx.InitLogger(opt...)
	return err
}

func (s ServerWrapper) InitApp() error {
	//lock main thread
	runtime.LockOSThread()

	// 加载应用svrno配置，logger初始化需要使用
	err := s.svr.LoadLocalConfig()
	if err != nil {
		return err
	}

	opts, err := s.svr.GetLogOptions()
	if err != nil {
		return err
	}
	s.initLogger(opts...)

	// 其他的游戏配置
	err = s.svr.LoadGameConfig()
	if err != nil {
		return err
	}

	err = s.svr.InitNetwork()
	if err != nil {
		return err
	}

	err = s.svr.InitDatabase()
	if err != nil {
		return err
	}

	s.svr.RegisterTimer()

	err = s.svr.InitFinish()
	if err != nil {
		return err
	}

	return nil
}

func (s ServerWrapper) Run() {
	s.svr.MainLoop()
}

func (s ServerWrapper) UnIntApp() {
	s.svr.UnInitDatabase()
	s.svr.UnInitNetwork()
	s.svr.UnInitFinish()

	runtime.UnlockOSThread()
}
