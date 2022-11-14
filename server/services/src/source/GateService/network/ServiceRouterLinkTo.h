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

#ifndef __SERVICEROUTERLINKTO_H__
#define __SERVICEROUTERLINKTO_H__

#include <cmsLib/net/LinkToBase.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

class ServiceRouterLinkTo : public LinkToBase
{
public:
	ServiceRouterLinkTo();
	ServiceRouterLinkTo(ServiceNodeInfo* pnode);

	virtual void connect();

	void reset(ServiceNodeInfo* pnode);
	virtual S_INT_64 get_iid() { return node_->iid; }
	virtual S_TIMESTAMP get_token() { return node_->token; }

	virtual std::string get_ext_bykey(const char* key);
	void heart_beat();

	virtual void force_linkclose();

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	void send_netprotocol(PRO::ERK_SERVICETYPE to, BasicProtocol* msg);
	void send_netprotocol(PRO::ERK_SERVICETYPE to, NetProtocol* pro);

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread( NetProtocol* pro);
	virtual NetProtocol* get_livekeep_msg();

public:
    void on_connected( bool success);
    void on_authed( bool success);
    void on_disconnected();

protected:
	std::shared_ptr<ServiceNodeInfo>	node_;

	//–≠“ÈÕ∑
	SProtocolHead	s_head_;
};

#endif	//__SERVICEROUTERLINKTO_H__
