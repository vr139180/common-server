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

#ifndef __GAMESERVICELINKFROM_H__
#define __GAMESERVICELINKFROM_H__

#include <gameLib/commons/NetLinkFromBase.h>
#include "network/FightRouterSession.h"

class GameServiceLinkFrom : public NetLinkFromBase<FightRouterSession>
{
public:
	GameServiceLinkFrom();
	virtual ~GameServiceLinkFrom();

	virtual void registinfo_tolog( bool bregist =true);

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(NetProtocol* pro);
	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	void bind_region(S_INT_32 rid) { regionid_ = rid; }
	S_INT_32 get_bind_regionid() { return regionid_; }

public:
	void send_netprotocol(BasicProtocol* msg);

protected:
	SProtocolHead	s_head_;
	//gameservice 注册region之后，做个绑定
	S_INT_32		regionid_;
};

#endif	//__GAMESERVICELINKFROM_H__
