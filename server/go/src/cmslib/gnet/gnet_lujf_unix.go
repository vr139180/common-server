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

//go:build linux || freebsd || dragonfly || darwin
// +build linux freebsd dragonfly darwin

package gnet

import (
	"cmslib/gnet/internal/socket"
	"cmslib/gnet/pkg/errors"
	errorsx "errors"
	"fmt"
	"net"
	"syscall"

	"golang.org/x/sys/unix"
)

// SH:Lujf:SH
// 连接到其他服务器的connect,为了公用eventloop处理机制
func (s Server) ConnectTo(ip string, port int, ns NetSession) error {
	address := fmt.Sprintf("%s:%d", ip, port)

	c, err := net.Dial("tcp", address)
	if err != nil {
		return err
	}

	if _, ok := c.(*net.TCPConn); !ok {
		return errors.ErrClientOnlySupportTCP
	}

	defer c.Close()

	sc, ok := c.(syscall.Conn)
	if !ok {
		return errorsx.New("failed to convert net.Conn to syscall.Conn")
	}
	rc, err := sc.SyscallConn()
	if err != nil {
		return errorsx.New("failed to get syscall.RawConn from net.Conn")
	}

	var DupFD int
	e := rc.Control(func(fd uintptr) {
		DupFD, err = unix.Dup(int(fd))
	})
	if err != nil {
		return err
	}
	if e != nil {
		return e
	}

	if err = socket.SetNoDelay(DupFD, 1); err != nil {
		return err
	}

	el := s.svr.lb.next(c.RemoteAddr())
	var sockAddr unix.Sockaddr
	if sockAddr, _, _, _, err = socket.GetTCPSockAddr(c.RemoteAddr().Network(), c.RemoteAddr().String()); err != nil {
		return err
	}

	conn := newTCPConn(DupFD, el, sockAddr, s.svr.opts.Codec, c.LocalAddr(), c.RemoteAddr())
	// bind each other
	conn.SetContext(ns)
	ns.SetConn(conn)

	err = el.poller.UrgentTrigger(el.register, conn)

	return nil
}

// SH:Lujf:SH
