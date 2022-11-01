package eureka

import (
	"cmslib/netx"
	"cmslib/protocolx"
	"cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"errors"
	"gamelib/service"
	"sync"

	"github.com/emirpasic/gods/lists/arraylist"
	"google.golang.org/protobuf/proto"
)

// eureka通知消息
type IEurekaNotify interface {
	//服务注册成功
	OnServiceRegisted(iid int64)
	//服务注册丢失，需要重新注册。包括丢失所有注册连接产生的丢失
	OnEurekaLosted()
	//订阅的服务发生变化
	OnServiceChanged(stype service.ServiceType, newiids []*ServiceNodeInfo, deliids []int64)
}

// 服务节点信息
type ServiceNodeInfo struct {
	SvrType service.ServiceType
	Iid     int64
	Token   int64
	Ip      string
	Port    int
	Exts    map[string]string
}

func (sn *ServiceNodeInfo) CloneExtParams() (r map[string]string) {
	r = make(map[string]string)
	if sn.Exts != nil && len(sn.Exts) > 0 {
		for k, v := range sn.Exts {
			r[k] = v
		}
	}

	return
}

func (sn *ServiceNodeInfo) GetExtParamByKey(k string) (v string, ok bool) {
	if sn.Exts == nil {
		ok = false
		return
	}

	v, ok = sn.Exts[k]
	return
}

// 构建一个service节点描述信息
func newServiceNodeInfo(s service.ServiceType, tid int64, token int64, ip string, port int, exts map[string]string) (n *ServiceNodeInfo) {
	n = new(ServiceNodeInfo)
	n.SvrType = s
	n.Iid = tid
	n.Token = token
	n.Ip = ip
	n.Port = port
	n.Exts = exts

	return n
}

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

	subscribes   map[int]void                //订阅类型
	serviceNodes map[int]ServiceNodesType    //service类型节点
	eurekaNodes  map[int64]*eurekaServerNode //存在的eureka服务节点

	eurekaVector      *arraylist.List
	curEurekaIndex    int
	eurekaConnections map[int64]*EurekaSession    //连接的session
	waitConnections   map[int64]*eurekaServerNode //等待连接的node
	authsConnection   map[*EurekaSession]void     //存放等待完成认证的session

	timerContainer *timerx.TimerContainer //软件定时器
	ch             chan interface{}
	once           sync.Once
	loopWG         sync.WaitGroup

	appProxy IEurekaNotify
	mynode   *ServiceNodeInfo
	//当前状态
	curState eurekaState
}

func NewEurekaCluster(ts *netx.TCPServer, s service.ServiceType, ip string, port int, exts map[string]string, subscribes []int, appProxy IEurekaNotify) (ec *EurekaCluster) {
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

	ec.subscribes = make(map[int]void)
	for _, v := range subscribes {
		ec.subscribes[v] = setEmptyMemeber
	}
	ec.serviceNodes = make(map[int]ServiceNodesType)

	ec.eurekaNodes = make(map[int64]*eurekaServerNode)

	ec.eurekaVector = arraylist.New()
	ec.curEurekaIndex = 0

	ec.eurekaConnections = make(map[int64]*EurekaSession)
	ec.waitConnections = make(map[int64]*eurekaServerNode)
	ec.authsConnection = make(map[*EurekaSession]void)

	ec.timerContainer = timerx.NewTimerContainer(500)

	ec.registTimer()

	return
}

func (ec *EurekaCluster) Start(eip string, eport int) {

	// 构建注册节点完成服务注册
	nod := &eurekaServerNode{iid: 0, ip: eip, port: eport}
	ec.waitConnections[nod.iid] = nod

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
	ec.timerContainer.AddTimer(2.5*1000, ec.timerAutoConnect)
	//5s自动同步
	ec.timerContainer.AddTimer(3*1000, ec.timerSyncEureka)
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
	pro := protocolx.NewNetProtocolByMsg(msg)
	ec.SendMessage(pro, balance)
}

func (ec *EurekaCluster) SendMessage(msg *protocolx.NetProtocol, balance bool) {
	l := ec.eurekaVector.Size()
	if l == 0 {
		return
	}

	if ec.curEurekaIndex >= l {
		ec.curEurekaIndex = 0
	}
	v, ok := ec.eurekaVector.Get(ec.curEurekaIndex)
	if ok {
		v.(*EurekaSession).SendMessage(msg)
	}

	if balance {
		ec.curEurekaIndex++
	}
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
