package processor

import (
	"cmslib/datas"
	"cmslib/gnet"
	"cmslib/utilc"
	"sync/atomic"
)

//登陆认证processor
type LoginProcessor struct {
	gnet.BaseNetSession
	datas.DoubleLinkBase

	//认证信息
	LoginType int
	//token loginuserid+usertoken
	LoginUserId int64
	UserToken   string
	//db account+pwd
	UserAccount string
	AccountPwd  string

	//分配开始时间
	AssignTime int64
	//注册请求时间
	RegistRequestTime int64
	//连接时间
	connectedTime int64
	//用户id
	UserIid int64
	//seqiid
	SeqIid int64
	//logintoken 用来登陆login的token
	LoginToken string

	//网络连接导致的 0-normal 1-closed，processor是否失效状态， atomic管理。
	isNetClosed int32
}

// 构建一个和tcp连接关联的session处理对象
func NewLoginProcessor(c gnet.Conn) (p *LoginProcessor) {
	p = new(LoginProcessor)
	p.SetConn(c)
	c.SetContext(p)

	//连接时的时间戳
	p.connectedTime = utilc.GetTimestamp()
	p.RegistRequestTime = 0

	atomic.StoreInt32(&p.isNetClosed, 0)

	p.isNetClosed = 0
	p.UserIid = 0

	return p
}

func (p *LoginProcessor) Login(uid int64, ut string) {
	p.UserIid = uid
	p.LoginToken = ut
}

func (p *LoginProcessor) IsValidProcessor() (ret bool) {
	ic := atomic.LoadInt32(&p.isNetClosed)
	ret = (ic == 0)
	return
}
