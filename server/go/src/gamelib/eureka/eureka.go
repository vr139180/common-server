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

package eureka

import (
	"cmslib/netx"
	"cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"encoding/json"
	"errors"
	"gamelib/protobuf/gpro"
	"gamelib/service"
	"io/ioutil"
	"net/http"
	"sync"

	"github.com/emirpasic/gods/lists/arraylist"
	"google.golang.org/protobuf/proto"
)

var errEureakClusterShutdown = errors.New("eureak cluster shutdown")

type void struct{}

var setEmptyMemeber void

type eurekaState int

const (
	eurekaWaitInit eurekaState = iota
	eurekaRegisted
	eurekaReady
	eurekaLosted //所有连接都丢失了
)

type ServiceNodesType map[int64]*ServiceNodeInfo

// eureka节点
// 需要处理的几个点：
// 1.eureka所有节点都丢失连接之后，通知app注册失败
// 2.app注册成功之后，app才可以提供服务
// 3.注册成功之后，1情况的链接丢失，后续进行服务的重新注册
type EurekaCluster struct {
	tcpServer *netx.TCPServer

	subscribe_services map[int]void              //订阅类型
	subscribe_balances map[int]void              //负载均衡的订阅类型
	serviceNodes       map[int]ServiceNodesType  //service类型节点
	eurekaNodes        map[int64]*EurekaNodeInfo //存在的eureka服务节点

	//master节点的 id+token
	masterEurekaIid   int64
	masterEurekaToken int64
	//master节点的链接
	masterLink *EurekaSession

	curEurekaIndex    int
	eurekaVector      *arraylist.List
	eurekaConnections map[int64]*EurekaSession //完成认证的session

	waitConnections map[int64]*EurekaNodeInfo //等待连接的node
	authsConnection map[*EurekaSession]void   //存放等待完成认证的session

	//本服务信息
	mynode *ServiceNodeInfo
	//当前状态
	curState eurekaState
	//是否router节点
	IsRouterNode bool

	timerContainer *timerx.TimerContainer //软件定时器
	ch             chan interface{}
	once           sync.Once
	loopWG         sync.WaitGroup

	appProxy IEurekaNotify
}

func GetEurekaMasterInfo(url string) (succ bool, node *EurekaNodeInfo) {
	succ = false
	node = nil

	resp, err := http.Get(url)
	if err != nil {
		return
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return
	}

	var md map[string]interface{}
	if err := json.Unmarshal(body, &md); err != nil {
		return
	}

	code, ok := md["code"]
	if !ok {
		return
	}
	if code.(int) != 0 {
		return
	}

	data, ok := md["data"]
	if !ok {
		return
	}

	mdata := data.(map[string]interface{})

	node = newEurekaNodeInfo2(mdata)
	succ = true
	return
}

func NewEurekaCluster(ts *netx.TCPServer, s service.ServiceType, ip string, port int, exts map[string]string,
	enode *EurekaNodeInfo, subscribes []int, balances []int, isrouter bool, appProxy IEurekaNotify) (ec *EurekaCluster) {

	ec = new(EurekaCluster)
	ec.tcpServer = ts
	ec.ch = make(chan interface{}, 64)
	ec.curState = eurekaWaitInit
	ec.appProxy = appProxy

	ec.mynode = new(ServiceNodeInfo)
	ec.mynode.SvrType = s
	ec.mynode.Ip = ip
	ec.mynode.Port = port
	ec.mynode.Exts = exts

	ec.subscribe_services = make(map[int]void)
	for _, v := range subscribes {
		ec.subscribe_services[v] = setEmptyMemeber
	}
	ec.subscribe_balances = make(map[int]void)
	for _, v := range balances {
		ec.subscribe_balances[v] = setEmptyMemeber
	}

	ec.masterLink = nil

	ec.curEurekaIndex = 0
	ec.serviceNodes = make(map[int]ServiceNodesType)

	ec.eurekaNodes = make(map[int64]*EurekaNodeInfo)

	ec.eurekaVector = arraylist.New()

	ec.eurekaConnections = make(map[int64]*EurekaSession)
	ec.waitConnections = make(map[int64]*EurekaNodeInfo)
	ec.authsConnection = make(map[*EurekaSession]void)

	ec.timerContainer = timerx.NewTimerContainer(500)

	ec.registTimer()

	// 构建注册节点完成服务注册
	enode.ismaster = true
	ec.waitConnections[enode.iid] = enode

	return
}

func (ec *EurekaCluster) Start() {

	ec.loopWG.Add(1)
	go func() {
		ec.run()
	}()
}

func (ec *EurekaCluster) Shutdown() {
	ec.once.Do(func() {
		ec.ch <- errEureakClusterShutdown
		close(ec.ch)

		ec.loopWG.Wait()
	})
}

func (ec *EurekaCluster) RegistNetCmd(cmd server.ICommandBase) {
	ec.ch <- cmd
}

func (ec *EurekaCluster) registTimer() {
	//3s自动连接
	ec.timerContainer.AddTimer(3*1000, ec.timerAutoConnect)
	//5s自动同步
	ec.timerContainer.AddTimer(8*1000, ec.timerServiceAlive)
}

func (ec *EurekaCluster) run() {
	var err error
	defer func() {
		ec.timerContainer.Stop()
		ec.loopWG.Done()
	}()

	//启动定时器
	ec.timerContainer.Start()
	ec.timerContainer.SetRealState(true)

	for {
		select {
		case i := <-ec.ch: //其他命令
			switch v := i.(type) {
			case error:
				err = v
			case server.ICommandBase:
				v.Run()
			}
		case <-ec.timerContainer.Time.C: //定时器响应
			now := utilc.GetTimestamp()
			ec.timerContainer.Tick(now)
		}

		if err == errEureakClusterShutdown {
			break
		}
	}
}

func (ec *EurekaCluster) GetMyNode() *ServiceNodeInfo {
	return ec.mynode
}

func (ec *EurekaCluster) SendNetProtocol(msg proto.Message, balance bool) {
	l := ec.eurekaVector.Size()
	if l == 0 {
		return
	}

	if ec.curEurekaIndex >= l {
		ec.curEurekaIndex = 0
	} else if ec.curEurekaIndex < 0 {
		ec.curEurekaIndex = 0
	}

	v, ok := ec.eurekaVector.Get(ec.curEurekaIndex)
	if !ok {
		return
	}

	v.(*EurekaSession).SendToEureka(msg)

	if balance {
		ec.curEurekaIndex++
	}
}

func (ec *EurekaCluster) SendToMaster(msg proto.Message) {
	if ec.masterLink == nil {
		return
	}

	ec.masterLink.SendToEureka(msg)
}

func (ec *EurekaCluster) getServiceNodesByType(t int) ServiceNodesType {
	n, ok := ec.serviceNodes[t]
	if !ok {
		n = make(ServiceNodesType)
		ec.serviceNodes[t] = n
	}

	return n
}

func (ec *EurekaCluster) IsRegisted() bool {
	return ec.curState == eurekaRegisted || ec.curState == eurekaReady
}

func (ec *EurekaCluster) IsReady() bool {
	return ec.curState == eurekaReady
}

func (ec *EurekaCluster) IsExistEurekaNode(iid int64) bool {
	_, ok := ec.eurekaNodes[iid]
	return ok
}

func (ec *EurekaCluster) IsOnlineEurekaNode(iid int64) bool {
	_, ok := ec.eurekaConnections[iid]
	return ok
}

func (ec *EurekaCluster) HasEurekaMasterNode() bool {
	return ec.masterLink != nil
}

func (ec *EurekaCluster) SubscribeToMasterNode() {
	if len(ec.subscribe_services) > 0 {
		req := &gpro.Erk_ServiceSubscribeReq{}
		req.Myiid = ec.mynode.Iid
		req.Mysvrtype = int32(ec.mynode.SvrType)

		for k, v := range ec.subscribe_services {

		}

		ec.SendToMaster(req)
	}

	if len(ec.subscribe_balances) > 0 {
		req := &gpro.Erk_RouterSubscribeReq{}
		req.Myiid = ec.mynode.Iid
		req.Mysvrtype = int32(ec.mynode.SvrType)

		ec.SendToMaster(req)
	}
}
