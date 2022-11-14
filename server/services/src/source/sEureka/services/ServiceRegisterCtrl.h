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

#ifndef __SERVICEREGISTERCTRL_H__
#define __SERVICEREGISTERCTRL_H__

#include <set>
#include <boost/scoped_array.hpp>
#include <boost/unordered_map.hpp>
#include <cmsLib/CommandListener.h>

#include <gameLib/service_type.h>

#include "network/ServiceLinkFromHolder.h"
#include "network/ServiceLinkFrom.h"

class ServiceRegisterCtrl : public MessageProcess
{
public:
	ServiceRegisterCtrl();
	virtual ~ServiceRegisterCtrl();

	bool init_ctrl();
	void uninit_ctrl();
	void init_timer();

	S_INT_64 get_serviceiid_seed() { return last_serviceiid_seed_; }
	void set_serviceiid_seed(S_INT_64 iid) { last_serviceiid_seed_ = iid; }

	//构建全量的服务信息
	BasicProtocol* master_syncall_servicenodes();
	void broadcast_to_allsvrs(BasicProtocol* msg);

protected:
	S_INT_64 make_next_serviceiid();
	//master切换或者其他不同步情况下，跳跃一个间隔解决可能的serviceid占用问题
	S_INT_64 make_next_serviceiid_fix(S_INT_64 incstep = 10000) { last_serviceiid_seed_ += incstep; return ++last_serviceiid_seed_; }

	ServiceNodeInfo* find_servicenode_byiid(S_INT_64 iid);

	//---------eureka端缓存的 各个类型的service的描述
	void service_mth_meta_release_all();

	std::list<ServiceNodeInfo*>& get_service_node_oftype(NETSERVICE_TYPE type);
	std::list<S_INT_64>& get_subscribes_oftype(NETSERVICE_TYPE type);
	std::list<S_INT_64>& get_routers_oftype(NETSERVICE_TYPE type);
	void add_service_to_subscribe(NETSERVICE_TYPE type, S_INT_64 svriid);
	void add_service_to_router(NETSERVICE_TYPE type, S_INT_64 svriid);
	void del_service_from_subscribe(NETSERVICE_TYPE type, S_INT_64 svriid);
	void del_service_from_router(NETSERVICE_TYPE type, S_INT_64 svriid);

	//注册一个新的服务
	ServiceNodeInfo* regist_one_service(ServiceNodeInfo& info, bool forceupdate = false);
	void offline_one_service(S_INT_64 iid);

	void notify_service_online(S_INT_64 iid);
	void notify_service_offline(S_INT_64 iid, NETSERVICE_TYPE ctype);

protected:
	//活动的service连接,只在主线程中使用,代码保证线程安全
	ServiceLinkFromHolder<ServiceLinkFrom>				service_mth_links_;

	//所有的服务节点信息
	boost::unordered_map<S_INT_64, ServiceNodeInfo>		all_service_nodes_;
	//服务分类信息
	boost::unordered_map<NETSERVICE_TYPE, std::list<ServiceNodeInfo*>>	servie_of_type_;
	//服务订阅信息 key:服务类型 value:订阅人列表
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>> service_of_subscribe_;
	//负载均衡订阅 key:服务类型 value:订阅人列表
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>> router_of_subscribe_;

	//服务编号生成种子
	S_INT_64	last_serviceiid_seed_;

public:
	void on_mth_disconnected_with_service(ServiceLinkFrom* plink);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage( NetProtocol* message, bool& autorelease) {}

	void on_mth_serviceregist_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_mth_servicebind_req(NetProtocol* pro, bool& autorelease, void* session);

	//服务订阅信息同步
	void on_mth_servicesync_ntf(NetProtocol* pro, bool& autorelease);

	void on_mth_servicesubscribe_req(NetProtocol* pro, bool& autorelease);
	void on_mth_routersubscribe_req(NetProtocol* pro, bool& autorelease);
	void on_mth_routeronline_req(NetProtocol* pro, bool& autorelease);

	void on_mth_serviceshutdown_ntf(NetProtocol* pro, bool& autorelease);
};


#endif //__SERVICEREGISTERCTRL_H__