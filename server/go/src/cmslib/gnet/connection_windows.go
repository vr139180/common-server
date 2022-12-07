// Copyright (c) 2019 Andy Pan
// Copyright (c) 2018 Joshua J Baker
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package gnet

import (
	"bytes"
	"encoding/binary"
	"errors"
	"net"
	"sync"

	bbPool "cmslib/gnet/pkg/pool/bytebuffer"
	rbPool "cmslib/gnet/pkg/pool/ringbuffer"
	"cmslib/gnet/pkg/ringbuffer"
	"cmslib/protocolx"

	"google.golang.org/protobuf/proto"
)

type stderr struct {
	c   *stdConn
	err error
}

type signalTask struct {
	run func(*stdConn) error
	c   *stdConn
}

type dataTask struct {
	run func([]byte) (int, error)
	buf []byte
}

type tcpConn struct {
	c  *stdConn
	bb *bbPool.ByteBuffer
}

type udpConn struct {
	c *stdConn
}

// SH:Lujf:SH
// integrate protobuf
type protobufTask struct {
	run func(*protocolx.NetProtocol) (int, error)
	pro *protocolx.NetProtocol
}

var (
	signalTaskPool = sync.Pool{New: func() interface{} { return new(signalTask) }}
	dataTaskPool   = sync.Pool{New: func() interface{} { return new(dataTask) }}
	// SH:Lujf:SH
	protobufTaskPool = sync.Pool{New: func() interface{} { return new(protobufTask) }}
)

type stdConn struct {
	ctx           interface{}            // user-defined context
	conn          net.Conn               // original connection
	loop          *eventloop             // owner event-loop
	buffer        *bbPool.ByteBuffer     // reuse memory of inbound data as a temporary buffer
	codec         ICodec                 // codec for TCP
	localAddr     net.Addr               // local server addr
	remoteAddr    net.Addr               // remote peer addr
	byteBuffer    *bbPool.ByteBuffer     // bytes buffer for buffering current packet and data in ring-buffer
	inboundBuffer *ringbuffer.RingBuffer // buffer for data from the peer
}

func packTCPConn(c *stdConn, buf []byte) *tcpConn {
	packet := &tcpConn{c: c}
	packet.bb = bbPool.Get()
	_, _ = packet.bb.Write(buf)
	return packet
}

func packUDPConn(c *stdConn, buf []byte) *udpConn {
	_, _ = c.buffer.Write(buf)
	packet := &udpConn{c: c}
	return packet
}

func newTCPConn(conn net.Conn, el *eventloop) (c *stdConn) {
	c = &stdConn{
		conn:          conn,
		loop:          el,
		codec:         el.svr.codec,
		inboundBuffer: rbPool.Get(),
	}
	c.localAddr = el.svr.ln.addr
	c.remoteAddr = c.conn.RemoteAddr()

	var (
		ok bool
		tc *net.TCPConn
	)
	if tc, ok = conn.(*net.TCPConn); !ok {
		return
	}
	var noDelay bool
	switch el.svr.opts.TCPNoDelay {
	case TCPNoDelay:
		noDelay = true
	case TCPDelay:
	}
	_ = tc.SetNoDelay(noDelay)
	if el.svr.opts.TCPKeepAlive > 0 {
		_ = tc.SetKeepAlive(true)
		_ = tc.SetKeepAlivePeriod(el.svr.opts.TCPKeepAlive)
	}
	return
}

func (c *stdConn) releaseTCP() {
	c.ctx = nil
	c.localAddr = nil
	c.remoteAddr = nil
	c.conn = nil
	rbPool.Put(c.inboundBuffer)
	c.inboundBuffer = ringbuffer.EmptyRingBuffer
	bbPool.Put(c.buffer)
	c.buffer = nil
}

func newUDPConn(el *eventloop, localAddr, remoteAddr net.Addr) *stdConn {
	return &stdConn{
		loop:       el,
		buffer:     bbPool.Get(),
		localAddr:  localAddr,
		remoteAddr: remoteAddr,
	}
}

func (c *stdConn) releaseUDP() {
	c.ctx = nil
	c.localAddr = nil
	bbPool.Put(c.buffer)
	c.buffer = nil
}

func (c *stdConn) read() ([]byte, error) {
	return c.codec.Decode(c)
}

func (c *stdConn) write(data []byte) (n int, err error) {
	if c.conn != nil {
		var outFrame []byte
		if outFrame, err = c.codec.Encode(c, data); err != nil {
			return
		}
		c.loop.eventHandler.PreWrite(c)
		n, err = c.conn.Write(outFrame)
		c.loop.eventHandler.AfterWrite(c, data)
	}
	return
}

// SH:Lujf:SH
// eventloop 中写消息到tcp流
func (c *stdConn) writeProtobuf(pro *protocolx.NetProtocol) (int, error) {
	if c.conn != nil && pro != nil {
		pcode, ok := c.codec.(*ProtobufCodec)
		if !ok {
			return 0, errors.New("codec not support ProtobufCodec")
		}

		head := pro.WriteHead()

		if head.UnpackProtocol {
			if pro.Msg == nil {
				return 0, errors.New("pro.Msg == nil")
			}

			id, err := pcode.Factory.ProtoToId(pro.Msg)
			if err != nil {
				return 0, err
			}

			head.MsgId = id
		}

		outFrame := bytes.NewBuffer([]byte{})

		head.EncodeHead(outFrame)

		if head.UnpackProtocol {
			data, err := proto.Marshal(pro.Msg)
			if err != nil {
				return 0, err
			}

			err = binary.Write(outFrame, binary.BigEndian, data)
			if err != nil {
				return 0, err
			}

			datalen := uint32(len(data))

			head.EncodeTotleLen(outFrame, datalen)
		} else {
			if pro.DataLen > 0 {
				err := binary.Write(outFrame, binary.BigEndian, pro.MsgData)
				if err != nil {
					return 0, err
				}

				head.EncodeTotleLen(outFrame, uint32(pro.DataLen))
			}
		}

		dats := outFrame.Bytes()

		c.loop.eventHandler.PreWrite(c)
		n, err := c.conn.Write(dats)
		c.loop.eventHandler.AfterWrite(c, dats)

		return n, err
	}

	return 0, nil
}

// ================================== Non-concurrency-safe API's ==================================

func (c *stdConn) Read() []byte {
	if c.inboundBuffer.IsEmpty() {
		return c.buffer.Bytes()
	}
	c.byteBuffer = c.inboundBuffer.WithByteBuffer(c.buffer.Bytes())
	return c.byteBuffer.Bytes()
}

func (c *stdConn) ResetBuffer() {
	c.buffer.Reset()
	c.inboundBuffer.Reset()
	bbPool.Put(c.byteBuffer)
	c.byteBuffer = nil
}

func (c *stdConn) ReadN(n int) (size int, buf []byte) {
	inBufferLen := c.inboundBuffer.Length()
	tempBufferLen := c.buffer.Len()
	if totalLen := inBufferLen + tempBufferLen; totalLen < n || n <= 0 {
		n = totalLen
	}
	size = n
	if c.inboundBuffer.IsEmpty() {
		buf = c.buffer.B[:n]
		return
	}
	head, tail := c.inboundBuffer.Peek(n)
	c.byteBuffer = bbPool.Get()
	_, _ = c.byteBuffer.Write(head)
	_, _ = c.byteBuffer.Write(tail)
	if inBufferLen >= n {
		buf = c.byteBuffer.Bytes()
		return
	}

	restSize := n - inBufferLen
	_, _ = c.byteBuffer.Write(c.buffer.B[:restSize])
	buf = c.byteBuffer.Bytes()
	return
}

func (c *stdConn) ShiftN(n int) (size int) {
	inBufferLen := c.inboundBuffer.Length()
	tempBufferLen := c.buffer.Len()
	if inBufferLen+tempBufferLen < n || n <= 0 {
		c.ResetBuffer()
		size = inBufferLen + tempBufferLen
		return
	}
	size = n
	if c.inboundBuffer.IsEmpty() {
		c.buffer.B = c.buffer.B[n:]
		return
	}

	bbPool.Put(c.byteBuffer)
	c.byteBuffer = nil

	if inBufferLen >= n {
		c.inboundBuffer.Discard(n)
		return
	}
	c.inboundBuffer.Reset()

	restSize := n - inBufferLen
	c.buffer.B = c.buffer.B[restSize:]
	return
}

func (c *stdConn) BufferLength() int {
	return c.inboundBuffer.Length() + c.buffer.Len()
}

func (c *stdConn) Context() interface{}       { return c.ctx }
func (c *stdConn) SetContext(ctx interface{}) { c.ctx = ctx }
func (c *stdConn) LocalAddr() net.Addr        { return c.localAddr }
func (c *stdConn) RemoteAddr() net.Addr       { return c.remoteAddr }

// ==================================== Concurrency-safe API's ====================================

//+ SH:Lujf:SH
// 发送protobuf协议，在发送时构造字节流
func (c *stdConn) AsyncWriteProtobuf(pro *protocolx.NetProtocol) error {
	task := protobufTaskPool.Get().(*protobufTask)
	task.run = c.writeProtobuf
	task.pro = pro
	c.loop.ch <- task
	return nil
}

//- SH:Lujf:SH

func (c *stdConn) AsyncWrite(buf []byte) error {
	task := dataTaskPool.Get().(*dataTask)
	task.run = c.write
	task.buf = buf
	c.loop.ch <- task
	return nil
}

func (c *stdConn) AsyncWritev(bs [][]byte) error {
	for _, b := range bs {
		_ = c.AsyncWrite(b)
	}
	return nil
}

func (c *stdConn) SendTo(buf []byte) (err error) {
	c.loop.eventHandler.PreWrite(c)
	_, err = c.loop.svr.ln.packetConn.WriteTo(buf, c.remoteAddr)
	c.loop.eventHandler.AfterWrite(c, buf)
	return
}

func (c *stdConn) Wake() error {
	task := signalTaskPool.Get().(*signalTask)
	task.run = c.loop.wake
	task.c = c
	c.loop.ch <- task
	return nil
}

func (c *stdConn) Close() error {
	task := signalTaskPool.Get().(*signalTask)
	task.run = c.loop.closeConn
	task.c = c
	c.loop.ch <- task
	return nil
}
