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

#ifndef __SERVICELINKFROM_H__
#define __SERVICELINKFROM_H__

#include <cmsLib/core_type.h>
#include <gameLib/service_type.h>
#include <gameLib/eureka/ServiceNodeInfo.h>
#include <gameLib/commons/NetLinkFromBase.h>
#include "network/EurekaSession.h"

class ServiceLinkFrom : public NetLinkFromBase<EurekaSession>
{
public:
	ServiceLinkFrom();

	void set_node(ServiceNodeInfo* pnode);
	ServiceNodeInfo* get_node() { return &node_; }

	virtual void reset();
	virtual void registinfo_tolog( bool bregist =true);
	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	void send_to_service(BasicProtocol* msg);

public:
	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);

	virtual S_INT_64 get_iid() { return node_.iid; }
	virtual S_INT_64 get_token() { return node_.token; }

protected:
	ServiceNodeInfo	node_;

	//协议头缺省
	SProtocolHead	s_head_;
};

#endif	//__SERVICELINKFROM_H__
