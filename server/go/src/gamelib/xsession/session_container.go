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

package xsession

import (
	"cmslib/gnet"
	"cmslib/protocolx"
	"gamelib/protobuf/gpro"
	"gamelib/service"
	"sync"
)

type NetSessionContainer struct {
	sessions map[int64]gnet.NetSession
	cond     *sync.Cond
	SType    service.ServiceType
	SIid     int64
}

func NewNetSessionContainer(s service.ServiceType) (c *NetSessionContainer) {
	c = new(NetSessionContainer)
	c.SType = s
	c.SIid = 0
	c.sessions = make(map[int64]gnet.NetSession)
	c.cond = sync.NewCond(&sync.Mutex{})

	return c
}

func (c *NetSessionContainer) DeleteOneSession(iid int64) {
	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()
	delete(c.sessions, iid)
}

func (c *NetSessionContainer) GetSessionByIid(iid int64) (n gnet.NetSession, ok bool) {
	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()
	n, ok = c.sessions[iid]

	return
}

func (c *NetSessionContainer) SendMessage(iid int64, msg *protocolx.NetProtocol) bool {
	n, ok := c.GetSessionByIid(iid)
	if !ok {
		return ok
	}

	n.SendMessage(msg)

	return true
}

func (c *NetSessionContainer) RegistNetSession(req *gpro.Svr_ServiceBindServiceReq, ns gnet.NetSession) {
	if req.GetToiid() != c.SIid {
		//disconnect
		ack := &gpro.Svr_ServiceBindServiceAck{}
		ack.Result = 1
		ns.SendMessage(ack)
		ns.Close()
		return
	}

	defer func() {
		c.cond.L.Unlock()
	}()

	c.cond.L.Lock()

	//save ext params of service
	ns.SetExtParams(req.GetMyexts())
	c.sessions[req.GetMyiid()] = ns

	ack := &gpro.Svr_ServiceBindServiceAck{}
	ack.Result = 0
	ack.SvrType = req.SvrType
	ack.Toiid = req.Toiid
	ack.Totoken = req.Totoken
	ns.SendMessage(ack)
}
