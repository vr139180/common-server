// Copyright (c) 2021 Andy Pan
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
	"context"
	"errors"
	"net"
	"strconv"
	"strings"
	"sync"
	"syscall"
	"time"

	"golang.org/x/sys/unix"

	"cmslib/gnet/internal/netpoll"
	"cmslib/gnet/internal/socket"
	"cmslib/gnet/internal/toolkit"
	gerrors "cmslib/gnet/pkg/errors"
	"cmslib/gnet/pkg/logging"
)

// Client of gnet.
type Client struct {
	opts     *Options
	el       *eventloop
	logFlush func() error
}

// NewClient creates an instance of Client.
func NewClient(eventHandler EventHandler, opts ...Option) (cli *Client, err error) {
	options := loadOptions(opts...)
	cli = new(Client)
	cli.opts = options
	var logger logging.Logger
	if options.LogPath != "" {
		if logger, cli.logFlush, err = logging.CreateLoggerAsLocalFile(options.LogPath, options.LogLevel); err != nil {
			return
		}
	} else {
		logger = logging.GetDefaultLogger()
	}
	if options.Logger == nil {
		options.Logger = logger
	}
	if options.Codec == nil {
		cli.opts.Codec = new(BuiltInFrameCodec)
	}
	var p *netpoll.Poller
	if p, err = netpoll.OpenPoller(); err != nil {
		return
	}
	svr := new(server)
	svr.opts = options
	svr.eventHandler = eventHandler
	svr.ln = &listener{network: "udp"}
	svr.cond = sync.NewCond(&sync.Mutex{})
	if options.Ticker {
		svr.tickerCtx, svr.cancelTicker = context.WithCancel(context.Background())
	}
	el := new(eventloop)
	el.ln = svr.ln
	el.svr = svr
	el.poller = p
	if rbc := options.ReadBufferCap; rbc <= 0 {
		options.ReadBufferCap = 0x10000
	} else {
		options.ReadBufferCap = toolkit.CeilToPowerOfTwo(rbc)
	}
	el.buffer = make([]byte, options.ReadBufferCap)
	el.udpSockets = make(map[int]*conn)
	el.connections = make(map[int]*conn)
	el.eventHandler = eventHandler
	cli.el = el
	return
}

// Start starts the client event-loop, handing IO events.
func (cli *Client) Start() error {
	cli.el.eventHandler.OnInitComplete(Server{})
	cli.el.svr.wg.Add(1)
	go func() {
		cli.el.run(cli.opts.LockOSThread)
		cli.el.svr.wg.Done()
	}()
	// Start the ticker.
	if cli.opts.Ticker {
		go cli.el.ticker(cli.el.svr.tickerCtx)
	}
	return nil
}

// Stop stops the client event-loop.
func (cli *Client) Stop() (err error) {
	err = cli.el.poller.UrgentTrigger(func(_ interface{}) error { return gerrors.ErrServerShutdown }, nil)
	cli.el.svr.wg.Wait()
	cli.el.poller.Close()
	cli.el.eventHandler.OnShutdown(Server{})
	// Stop the ticker.
	if cli.opts.Ticker {
		cli.el.svr.cancelTicker()
	}
	if cli.logFlush != nil {
		err = cli.logFlush()
	}
	logging.Cleanup()
	return
}

// Dial is like net.Dial().
func (cli *Client) Dial(network, address string) (Conn, error) {
	c, err := net.Dial(network, address)
	if err != nil {
		return nil, err
	}
	defer c.Close()

	sc, ok := c.(syscall.Conn)
	if !ok {
		return nil, errors.New("failed to convert net.Conn to syscall.Conn")
	}
	rc, err := sc.SyscallConn()
	if err != nil {
		return nil, errors.New("failed to get syscall.RawConn from net.Conn")
	}

	var DupFD int
	e := rc.Control(func(fd uintptr) {
		DupFD, err = unix.Dup(int(fd))
	})
	if err != nil {
		return nil, err
	}
	if e != nil {
		return nil, e
	}

	if strings.HasPrefix(network, "tcp") {
		if cli.opts.TCPNoDelay == TCPNoDelay {
			if err = socket.SetNoDelay(DupFD, 1); err != nil {
				return nil, err
			}
		}
		if cli.opts.TCPKeepAlive > 0 {
			if err = socket.SetKeepAlive(DupFD, int(cli.opts.TCPKeepAlive/time.Second)); err != nil {
				return nil, err
			}
		}
	}

	if cli.opts.SocketSendBuffer > 0 {
		if err = socket.SetSendBuffer(DupFD, cli.opts.SocketSendBuffer); err != nil {
			return nil, err
		}
	}
	if cli.opts.SocketRecvBuffer > 0 {
		if err = socket.SetRecvBuffer(DupFD, cli.opts.SocketRecvBuffer); err != nil {
			return nil, err
		}
	}

	var (
		sockAddr unix.Sockaddr
		gc       Conn
	)
	switch c.(type) {
	case *net.UnixConn:
		if sockAddr, _, _, err = socket.GetUnixSockAddr(c.RemoteAddr().Network(), c.RemoteAddr().String()); err != nil {
			return nil, err
		}
		ua := c.LocalAddr().(*net.UnixAddr)
		ua.Name = c.RemoteAddr().String() + "." + strconv.Itoa(DupFD)
		gc = newTCPConn(DupFD, cli.el, sockAddr, cli.opts.Codec, c.LocalAddr(), c.RemoteAddr())
	case *net.TCPConn:
		if sockAddr, _, _, _, err = socket.GetTCPSockAddr(c.RemoteAddr().Network(), c.RemoteAddr().String()); err != nil {
			return nil, err
		}
		gc = newTCPConn(DupFD, cli.el, sockAddr, cli.opts.Codec, c.LocalAddr(), c.RemoteAddr())
	case *net.UDPConn:
		if sockAddr, _, _, _, err = socket.GetUDPSockAddr(c.RemoteAddr().Network(), c.RemoteAddr().String()); err != nil {
			return nil, err
		}
		gc = newUDPConn(DupFD, cli.el, c.LocalAddr(), sockAddr, true)
	default:
		return nil, gerrors.ErrUnsupportedProtocol
	}
	err = cli.el.poller.UrgentTrigger(cli.el.register, gc)
	if err != nil {
		gc.Close()
		return nil, err
	}
	return gc, nil
}
