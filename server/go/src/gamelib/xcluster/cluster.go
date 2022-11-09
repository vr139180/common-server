package xcluster

import (
	"cmslib/gnet"
	"cmslib/netx"
	"cmslib/protocolx"
	"cmslib/server"
	"cmslib/timerx"
	"cmslib/utilc"
	"gamelib/eureka"
	"gamelib/service"
	"sync"

	"github.com/emirpasic/gods/lists/arraylist"
	"google.golang.org/protobuf/proto"
)

type void struct{}

var setEmptyMemeber void

//netsession需要实现的cluster组件接口
type IClusterAppWrapper interface {
	NewNetSession(s service.ServiceType, p *ClusterServiceCtrl, node *eureka.ServiceNodeInfo) IClusterNetSession
	GetMyNode() *eureka.ServiceNodeInfo
}

type IClusterNetSession interface {
	GetTcpConn() gnet.Conn
	GetServiceNode() *eureka.ServiceNodeInfo
	GetNetSession() gnet.NetSession
	SendClusterMessage(*protocolx.NetProtocol)
}

//cluster服务控制器
type ClusterServiceCtrl struct {
	tcpServer *netx.TCPServer

	serviceNodes    map[int64]*eureka.ServiceNodeInfo
	waitConnections map[*eureka.ServiceNodeInfo]void
	authConnections map[IClusterNetSession]void
	//记录了有效的service信息
	serviceConnections map[int64]IClusterNetSession
	serviceVector      *arraylist.List
	curServiceIndex    int

	cond *sync.Cond

	mynode      *eureka.ServiceNodeInfo
	targetSType service.ServiceType
	appProxy    IClusterAppWrapper

	defaultSHead protocolx.SProtocolHead

	//---------------------------------------------------
	timerContainer *timerx.TimerContainer //软件定时器
	ch             chan interface{}
	once           sync.Once
	loopWG         sync.WaitGroup
}

func NewClusterServiceCtrl(ts *netx.TCPServer, s service.ServiceType, app IClusterAppWrapper, head protocolx.SProtocolHead) (c *ClusterServiceCtrl) {
	c = new(ClusterServiceCtrl)

	c.defaultSHead = head

	c.tcpServer = ts
	c.appProxy = app
	c.ch = make(chan interface{}, 64)
	//一秒一次基础tick
	c.timerContainer = timerx.NewTimerContainer(1000)

	c.waitConnections = make(map[*eureka.ServiceNodeInfo]void)
	c.cond = sync.NewCond(&sync.Mutex{})
	c.serviceNodes = make(map[int64]*eureka.ServiceNodeInfo)
	c.authConnections = make(map[IClusterNetSession]void)
	c.serviceConnections = make(map[int64]IClusterNetSession)
	c.serviceVector = arraylist.New()
	c.curServiceIndex = 0

	c.targetSType = s
	c.mynode = app.GetMyNode()

	//注册定时器
	c.registTimer()

	return
}

func (c *ClusterServiceCtrl) Start() {
	c.loopWG.Add(1)
	go func() {
		c.run()
	}()
}

func (c *ClusterServiceCtrl) Shutdown() {
	c.once.Do(func() {
		c.ch <- server.ErrExitApp
		close(c.ch)

		c.loopWG.Wait()
	})
}

func (c *ClusterServiceCtrl) registTimer() {
	//3s自动连接
	c.timerContainer.AddTimer(3*1000, c.timerAutoConnect)
}

func (c *ClusterServiceCtrl) run() {
	var err error
	defer func() {
		c.timerContainer.Stop()
		c.loopWG.Done()
	}()

	//启动定时器
	c.timerContainer.Start()

	for {
		select {
		case i := <-c.ch: //其他命令
			switch v := i.(type) {
			case error:
				err = v
			case server.ICommandBase:
				v.Run()
			}
		case <-c.timerContainer.Time.C: //定时器响应
			now := utilc.GetTimestamp()
			c.timerContainer.Tick(now)
		}

		if err == server.ErrExitApp {
			break
		}
	}
}

func (c *ClusterServiceCtrl) SendNetMessage(totype int8, token protocolx.UserToken, msg proto.Message) {

	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()
	l := c.serviceVector.Size()
	if l == 0 {
		return
	}

	if c.curServiceIndex >= l {
		c.curServiceIndex = 0
	}
	v, ok := c.serviceVector.Get(c.curServiceIndex)
	if ok {
		pro := protocolx.NewNetProtocolByHeadMsg(msg, &c.defaultSHead)
		head := pro.WriteHead()
		head.ToType = totype
		head.Token = token
		v.(IClusterNetSession).SendClusterMessage(pro)
	}

	c.curServiceIndex++
}

func (c *ClusterServiceCtrl) RealStartTimer(start bool) {
	c.timerContainer.SetRealState(start)
}

//------------------------------------------------------------
type ClusterNetCmdFunction = func(IClusterNetSession, *protocolx.NetProtocol)

type ClusterNetCmd struct {
	sess IClusterNetSession
	msg  *protocolx.NetProtocol
	fun  ClusterNetCmdFunction
}

func NewClusterNetCmd(s IClusterNetSession, m *protocolx.NetProtocol, f ClusterNetCmdFunction) (cmd *ClusterNetCmd) {
	cmd = new(ClusterNetCmd)
	cmd.sess = s
	cmd.msg = m
	cmd.fun = f

	return
}

func (n *ClusterNetCmd) Run() {
	if n.sess != nil && n.msg != nil && n.fun != nil {
		n.fun(n.sess, n.msg)
	}
}
