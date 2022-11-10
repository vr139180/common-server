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

#ifndef __EUREKALINKFROM_H__
#define __EUREKALINKFROM_H__

#include <gameLib/eureka/EurekaNodeInfo.h>
#include <gameLib/commons/NetLinkFromBase.h>

#include "network/IEurekaNodeLink.h"
#include "network/EurekaSession.h"

class EurekaLinkFrom : public NetLinkFromBase<EurekaSession>, public IEurekaNodeLink
{
public:
	EurekaLinkFrom();
	EurekaLinkFrom(EurekaNodeInfo pnode);

	void set_node(EurekaNodeInfo pnode) { node_ = pnode; }

	virtual void reset();

	virtual void registinfo_tolog( bool bregist =true);

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

public:
	//-----------------NetSessionBindEvent-------------------------------
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);

	//---------------IEurekaNodeLink-----------------------------
	virtual void send_to_eureka(BasicProtocol* pro);

	virtual bool is_ready() { return NetLinkFromBase::is_valid(); }
	virtual bool is_link_to() { return false; }
	virtual void force_linkclose();

	virtual S_INT_64 get_iid() { return node_.iid; }
	virtual S_INT_64 get_token() { return node_.token; }
	virtual void heart_beat();

protected:
	EurekaNodeInfo	node_;

	//协议头缺省
	SProtocolHead	s_head_;
};

#endif	//__EUREKALINKFROM_H__
