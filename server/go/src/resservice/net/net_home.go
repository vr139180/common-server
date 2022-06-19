package net

import (
	"cmslib/gnet"
	"cmslib/utilc"
	"resservice/g"

	"google.golang.org/protobuf/proto"
)

//home的netsession
type HomeNetSession struct {
	gnet.BaseNetSession

	//扩展参数
	extparams map[string]string

	//连接时间
	connectedTime int64
	SIid          int64
}

func NewHomeNetSession(c gnet.Conn, id int64) (l *HomeNetSession) {
	l = new(HomeNetSession)
	l.SetConn(c)
	c.SetContext(l)
	l.SIid = id

	l.connectedTime = utilc.GetTimestamp()

	return
}

//--------------------gnet.NetSession interface-----------------------
func (l *HomeNetSession) SetExtParams(ext map[string]string) {
	l.extparams = l.CloneExParams(ext)
}

func (l *HomeNetSession) GetExtParamByKey(k string) (string, bool) {
	if len(l.extparams) == 0 {
		return "", false
	}

	v, ok := l.extparams[k]
	return v, ok
}

func (l *HomeNetSession) OnRecvMessage(id int, pro proto.Message) {
	g.OnNetProcessor(l, id, pro)
}
