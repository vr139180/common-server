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

#ifndef __EUREKACLUSTERCLIENT_H__
#define __EUREKACLUSTERCLIENT_H__

#include <set>
#include <list>
#include <vector>
#include <boost/unordered_map.hpp>

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/CommandListener.h>
#include <cmsLib/ThreadLock.h>

#include <gameLib/service_type.h>
#include <gameLib/eureka/EurekaClusterLink.h>
#include <gameLib/eureka/IEurekaClientIntegrate.h>
#include <gameLib/eureka/EurekaNodeInfo.h>
#include <gameLib/eureka/ServiceNodeInfo.h>

typedef enum tagEurekaState {
	Eureka_WaitInit = 0,
	Eureka_Registed,
	Eureka_Done,
	Eureka_Losted
}EurekaState;

typedef boost::unordered_map<std::string, std::string> EurekaServerExtParam;

/**
* eureka注册类，单线程模式，运行在主线程中。
*
* @category
* @package
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2021
*/
class EurekaClusterClient : public MessageProcess
{
	friend class EurekaClusterLink;

	typedef boost::unordered_map<S_INT_64, ServiceNodeInfo*>	SERVICENODE_TYPE;
	typedef boost::unordered_map<S_INT_64, EurekaNodeInfo*>		EUREKANODE_TYPE;

public:
	static EurekaClusterClient& instance();

	static bool get_eureka_masterinfo(const char* eurekaurl, EurekaNodeInfo& info);

private:
	EurekaClusterClient();

public:
	virtual ~EurekaClusterClient();

	void init(IEurekaClientIntegrate* app, NETSERVICE_TYPE type, const char* myip, int myport, 
		EurekaServerExtParam extpms, EurekaNodeInfo& eureka, 
		std::list< NETSERVICE_TYPE>& subscribe_service,
		std::list< NETSERVICE_TYPE>& subscribe_balance,
		bool isrouter = false);
	void uninit();

	void regist_timer();

	bool is_registed() { return cur_state_ == EurekaState::Eureka_Registed || cur_state_ == EurekaState::Eureka_Done; }
	bool is_done() { return cur_state_ == EurekaState::Eureka_Done; }

	void send_mth_protocol(BasicProtocol* pro, bool balance = true);
	void send_to_master(BasicProtocol* pro);
	void regist_command(CommandBase *p);
	bool is_servicenode_exist(NETSERVICE_TYPE type, S_INT_64 sid);

	EurekaClusterLink* get_eurekalink_byiid(S_INT_64 iid);

	bool has_eureka_masternode() { return master_link_ != 0; }
	void subscribe_to_masternode();

protected:
	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease) {}

	void on_service_subscribe_ntf(NetProtocol* message, bool& autorelease);
	void on_router_subscribe_ntf(NetProtocol* message, bool& autorelease);
	void on_eurekaupdate_ntf(NetProtocol* message, bool& autorelease);
	void on_eurekamaster_change_ntf(NetProtocol* message, bool& autorelease);
	void on_routeronline_ntf(NetProtocol* pro, bool& autorelease);

protected:
	void auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void router_autoconfirm_timer(u64 tnow, int interval, u64 iid, bool& finish);

	void on_link_disconnected( EurekaClusterLink* plink);
	void on_link_regist_result( EurekaClusterLink* plink);
	void on_link_bind_result( EurekaClusterLink* plink);

public:
	NETSERVICE_TYPE get_svrtype() { return svrtype_; }
	S_INT_64	get_myiid() { return service_iid_; }
	S_INT_64	get_token() { return token_; }
	const char* get_myip() { return myip_.c_str(); }
	int			get_myport() { return myport_; }
	const EurekaServerExtParam& get_extps() { return extpms_; }

protected:
	void set_myinfo(S_INT_64 iid, S_INT_64 token) {
		this->service_iid_ = iid;
		this->token_ = token;
	}

	void release();

protected:
	bool is_eurekanode_exist(S_INT_64 sid);
	bool is_eurekanode_connected(S_INT_64 sid);
	SERVICENODE_TYPE& get_servicenodes_by_type(NETSERVICE_TYPE type);

	//同步的eureka节点信息，和订阅的service信息
	boost::unordered_map<S_INT_64, EurekaNodeInfo>	eureka_nodes_;
	boost::unordered_map<NETSERVICE_TYPE, SERVICENODE_TYPE > service_nodes_;

protected:
	//本服务信息
	S_INT_64				service_iid_;
	S_INT_64				token_;

	EurekaState				cur_state_;
	//本服务信息
	NETSERVICE_TYPE			svrtype_;
	std::string				myip_;
	int						myport_;
	EurekaServerExtParam	extpms_;
	S_INT_64				master_eureka_iid_;
	S_INT_64				master_eureka_token_;

	//和主线程的集成
	IEurekaClientIntegrate*			app_proxy_;
	TimerKey						auto_connect_tk_;

	//订阅的服务类型
	std::list<NETSERVICE_TYPE>		subscribe_services_;
	//负载均衡的订阅类型
	std::list<NETSERVICE_TYPE>		subscribe_balance_;
	//是否是router节点
	bool							is_router_node_;

	//cluster节点，连接相关变量
	std::list<EurekaClusterLink*>	clusterlinks_;
	//都是对clusterlinks的引用
	std::set<EurekaClusterLink*>	auth_links_;
	std::set<EurekaClusterLink*>	wait_links_;
	std::set<EurekaClusterLink*>	free_links_;

	//存放已连接的节点
	std::vector<EurekaClusterLink*>	online_links_;
	EurekaClusterLink*				master_link_;

	volatile int					cur_link_index_;
};

#endif //__EUREKACLUSTERCLIENT_H__
