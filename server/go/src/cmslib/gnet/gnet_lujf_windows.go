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
	"cmslib/gnet/pkg/errors"
	"fmt"
	"net"
	"time"
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

	el := s.svr.lb.next(c.RemoteAddr())
	conn := newTCPConn(c, el)
	// bind each other
	conn.SetContext(ns)
	ns.SetConn(conn)

	el.ch <- conn
	go func() {
		var buffer [0x10000]byte
		for {
			n, err := c.Read(buffer[:])
			if err != nil {
				_ = c.SetReadDeadline(time.Time{})
				el.ch <- &stderr{conn, err}
				return
			}
			el.ch <- packTCPConn(conn, buffer[:n])
		}
	}()

	return nil
}

// SH:Lujf:SH
