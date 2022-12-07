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

package netx

import (
	"cmslib/gnet"
	"cmslib/gnet/pkg/logging"
	"cmslib/logx"
	"cmslib/protocolx"
	"encoding/binary"
	"errors"
	"fmt"
	"time"

	"google.golang.org/protobuf/proto"
)

// 应用监听接口
type ITCPServerApp interface {
	GetProtoFactory() (fact gnet.IProtobufFactory, err error)
	OnTCPInitComplete()
	OnTCPShutdown()
	OnTCPOpened(c gnet.Conn)
	OnTCPClosed(c gnet.Conn, err error)
	OnRecvMessage(c gnet.Conn, m *protocolx.NetProtocol)
}

// TCP server
type TCPServer struct {
	*gnet.EventServer

	svr       gnet.Server //gnet server
	AppListen ITCPServerApp
	Codec     *gnet.ProtobufCodec //gnet codec
}

func NewTCPServer(app ITCPServerApp) (svr *TCPServer, err error) {
	if app == nil {
		return nil, errors.New("tcpserver.go NewTCPServer function cant use a nil parameter app")
	}

	var fact gnet.IProtobufFactory
	fact, err = app.GetProtoFactory()
	if err != nil {
		return nil, errors.New("server must implement ITCPServerApp.GetProtoFactory function")
	}

	svr = new(TCPServer)
	svr.AppListen = app
	svr.Codec, err = gnet.NewProtobufCodec(fact)

	return
}

// server 绑定端口并开始监听连接
func (t *TCPServer) Accept(ip string, port int, ignorelisten bool) (err error) {

	if t.Codec == nil {
		logx.Errorf("server must init protobuf codec for gnet middlue")
		return errors.New("server must init protobuf codec for gnet middlue")
	}

	cc, ok := interface{}(t.Codec).(gnet.ICodec)
	if !ok {
		logx.Errorf("Codec must implement gnet.ICodec interface")
		return errors.New("codec must implement gnet.ICodec interface")
	}

	addr := fmt.Sprintf("tcp://%s:%d", ip, port)
	var gnetLog logging.Logger = logx.GetDefaultLogger()

	err = gnet.Serve(t, addr, gnet.WithMulticore(true), gnet.WithTCPKeepAlive(time.Minute*5),
		gnet.WithReusePort(true), gnet.WithLogger(gnetLog), gnet.WithCodec(cc), gnet.WithIgnoreListen(ignorelisten))
	if err != nil {
		return err
	}

	return nil
}

// 创建连接的客户端
func (t *TCPServer) Connect(ip string, port int, ns gnet.NetSession) error {
	return t.svr.ConnectTo(ip, port, ns)
}

func (t *TCPServer) OnInitComplete(svr gnet.Server) (action gnet.Action) {
	t.svr = svr
	t.AppListen.OnTCPInitComplete()
	return
}

func (t *TCPServer) OnShutdown(svr gnet.Server) {
	t.AppListen.OnTCPShutdown()
}

func (t *TCPServer) OnOpened(c gnet.Conn) (out []byte, action gnet.Action) {
	t.AppListen.OnTCPOpened(c)
	return
}

func (t *TCPServer) OnClosed(c gnet.Conn, err error) (action gnet.Action) {
	t.AppListen.OnTCPClosed(c, err)
	return
}

func (t *TCPServer) React(packet []byte, c gnet.Conn) (out []byte, action gnet.Action) {
	// out must be setted nil
	// 根据协议定义来解析协议
	if len(packet) < 4 {
		return
	}
	buf := packet[0:]
	totle := uint32(len(buf))

	for totle >= 4 {
		prolen := binary.BigEndian.Uint32(buf)
		//字节流长度不够
		if totle < prolen {
			break
		}

		pro := protocolx.NewNetProtocol()
		head := pro.WriteHead()

		if !head.DecodeHead(buf, prolen) {
			break
		}

		probuf := buf[int(head.HeadLen):]
		if head.UnpackProtocol {
			msg, err := t.Codec.Factory.IdToProto(head.MsgId)
			if err != nil {
				logx.Errorf("get a undefined message[id:%d] from tcp steam", head.MsgId)
			}
			err = proto.Unmarshal(probuf, msg)
			if err != nil {
				logx.Errorf("unmarshal a message[id:%d] from tcp steam", head.MsgId)
			}

			pro.Msg = msg
		}

		t.AppListen.OnRecvMessage(c, pro)

		totle -= prolen
		if totle < 4 {
			break
		}

		buf = buf[prolen:]
	}

	return
}
