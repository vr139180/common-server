package net

import (
	"cmslib/gnet"
	"cmslib/utilc"
	"resservice/g"

	"google.golang.org/protobuf/proto"
)

//login的netsession
type LoginNetSession struct {
	gnet.BaseNetSession

	//扩展参数
	extparams map[string]string

	//连接时间
	connectedTime int64
	SIid          int64
}

func NewLoginNetSession(c gnet.Conn, id int64) (l *LoginNetSession) {
	l = new(LoginNetSession)
	l.SetConn(c)
	c.SetContext(l)
	l.SIid = id

	l.connectedTime = utilc.GetTimestamp()

	return
}

//--------------------gnet.NetSession interface-----------------------
func (l *LoginNetSession) SetExtParams(ext map[string]string) {
	l.extparams = l.CloneExParams(ext)
}

func (l *LoginNetSession) GetExtParamByKey(k string) (string, bool) {
	if len(l.extparams) == 0 {
		return "", false
	}

	v, ok := l.extparams[k]
	return v, ok
}

func (l *LoginNetSession) OnRecvMessage(id int, pro proto.Message) {
	g.OnNetProcessor(l, id, pro)
}
