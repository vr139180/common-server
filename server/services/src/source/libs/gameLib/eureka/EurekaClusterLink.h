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

#ifndef __EUREKACLUSTERLINK_H__
#define __EUREKACLUSTERLINK_H__

#include <cmsLib/net/LinkToBase.h>

#include <gameLib/eureka/EurekaNodeInfo.h>

class EurekaClusterClient;

class EurekaClusterLink : public LinkToBase
{
public:
	EurekaClusterLink(EurekaClusterClient* p, EurekaNodeInfo* info);
	virtual ~EurekaClusterLink();

	void connect();
	void reset(EurekaNodeInfo* pnode);

	virtual void force_linkclose();

	virtual std::string get_ext_bykey(const char* key) { return ""; }

	virtual void init_protocolhead();
	virtual const SProtocolHead& get_protocolhead() { return s_head_; }

	S_INT_64 get_iid() { return node_->iid; }
	EurekaNodeInfo* clone_node() {
		if (node_.get() == 0) return 0;
		return node_->clone();
	}

	void send_to_eureka(BasicProtocol* msg);

public:
	//---------------implement NetSessionBindEvent---------------
	virtual void on_cant_connectedto();
	virtual void on_connectedto_done();

	virtual void on_connect_lost_netthread();
	virtual void on_recv_protocol_netthread(NetProtocol* pro);

public:
	void on_connected(bool success);
	void on_disconnected();
	void on_regist_result(NetProtocol* message, bool& autorelease);
	void on_bind_result(bool success);

protected:
	std::unique_ptr<EurekaNodeInfo>	node_;

	//协议头
	SProtocolHead					s_head_;

	//连接错误次数
	int		fail_num_;
	EurekaClusterClient*			parent_;
};

#endif //__EUREKACLUSTERLINK_H__