// Copyright (c) 2020 Andy Pan
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

package socket

import (
	"net"
	"os"

	"golang.org/x/sys/unix"

	"cmslib/gnet/pkg/errors"
)

// GetUnixSockAddr the structured addresses based on the protocol and raw address.
func GetUnixSockAddr(proto, addr string) (sa unix.Sockaddr, family int, unixAddr *net.UnixAddr, err error) {
	unixAddr, err = net.ResolveUnixAddr(proto, addr)
	if err != nil {
		return
	}

	switch unixAddr.Network() {
	case "unix":
		sa, family = &unix.SockaddrUnix{Name: unixAddr.Name}, unix.AF_UNIX
	default:
		err = errors.ErrUnsupportedUDSProtocol
	}

	return
}

// udsSocket creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
// Argument `reusePort` indicates whether the SO_REUSEPORT flag will be assigned.
func udsSocket(proto, addr string, passive bool, sockOpts ...Option) (fd int, netAddr net.Addr, err error) {
	var (
		family int
		sa     unix.Sockaddr
	)

	if sa, family, netAddr, err = GetUnixSockAddr(proto, addr); err != nil {
		return
	}

	if fd, err = sysSocket(family, unix.SOCK_STREAM, 0); err != nil {
		err = os.NewSyscallError("socket", err)
		return
	}
	defer func() {
		if err != nil {
			_ = unix.Close(fd)
		}
	}()

	for _, sockOpt := range sockOpts {
		if err = sockOpt.SetSockOpt(fd, sockOpt.Opt); err != nil {
			return
		}
	}

	if err = os.NewSyscallError("bind", unix.Bind(fd, sa)); err != nil {
		return
	}

	if passive {
		// Set backlog size to the maximum.
		err = os.NewSyscallError("listen", unix.Listen(fd, listenerBacklogMaxSize))
	} else {
		err = os.NewSyscallError("connect", unix.Connect(fd, sa))
	}

	return
}
