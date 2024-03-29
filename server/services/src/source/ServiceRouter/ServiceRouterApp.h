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

#ifndef __SERVICEROUTERAPP_H__
#define __SERVICEROUTERAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/SessionMthHolder.h>
#include <gameLib/commons/LinkFromHolder.h>
#include <gameLib/commons/LinkToHolder.h>
#include <gameLib/commons/LinkFromConsistentHash.h>

#include "config/RouterConfig.h"

#include "network/RouterSession.h"

#include "network/GateServiceLinkFrom.h"
#include "network/ChatServiceLinkFrom.h"
#include "network/MailServiceLinkFrom.h"
#include "network/FriendServiceLinkFrom.h"
#include "network/DataRouterLinkTo.h"

class ServiceRouterApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	ServiceRouterApp();

public:
	static ServiceRouterApp& getInstance();
	virtual ~ServiceRouterApp();

	virtual void main_loop();

	RouterConfig* get_config() { return conf_.get(); }

public:

	void router_to_gate(NetProtocol* pro);

	void router_to_chat(S_INT_64 channelid, NetProtocol* pro);
	S_INT_64 get_chat_by_channelid(S_INT_64 cid);
	//don't delete pro
	void broadcast_to_gate(NetProtocol* pro) {
		gate_links_from_.broadcast(pro);
	}

	void router_to_mail(NetProtocol* pro);
	void router_to_friend(NetProtocol* pro);

	void broad_protocal_to_mails(BasicProtocol* msg) {
		mail_links_from_.broadcast(msg);
	}

public:

	//------------------------------implement NetAcceptorEvent ------------------------------//
	virtual NetAcceptorEvent::NetSessionPtr ask_free_netsession();
	virtual void accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err);

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };

	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE, std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);
	virtual void mth_notify_routerbalance_new(NETSERVICE_TYPE, std::list<S_INT_64>& svrs);

	virtual void mth_service_registed(S_INT_64 sid);
	virtual void mth_eureka_losted();

protected:
	virtual bool load_config();
	virtual bool pre_init();
	virtual bool init_network();
	virtual void uninit_network();
	virtual void uninit();

	virtual bool init_finish();

	virtual void register_timer();

	RouterConfig* load_routerconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool							is_ready_;

	//network
	std::shared_ptr<NetAcceptor>		acceptor_;
	SessionMthHolder<RouterSession>		session_from_;

	LinkToHolder<DataRouterLinkTo>					datarouter_link_mth_;
	LinkFromHolder<GateServiceLinkFrom>				gate_links_from_;

	//负载均衡
	LinkFromConsistentHash<ChatServiceLinkFrom>		chat_links_from_;
	LinkFromConsistentHash<MailServiceLinkFrom>		mail_links_from_;
	LinkFromConsistentHash<FriendServiceLinkFrom>	friend_links_from_;

	boost::scoped_ptr<RouterConfig>	conf_;

public:
	void on_connection_timeout(RouterSession* session);

	void on_mth_servicebindservice_req(NetProtocol* pro, bool& autorelease, void* session);

	void on_disconnected_with_datarouter(DataRouterLinkTo* plink);
	void on_datarouter_regist_result(DataRouterLinkTo* plink);

	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);
	void on_disconnected_with_chatservice(ChatServiceLinkFrom* plink);
	void on_disconnected_with_mailservice(MailServiceLinkFrom* plink);
	void on_disconnected_with_frdservice(FriendServiceLinkFrom* plink);
};

#define svrApp (ServiceRouterApp::getInstance())

#endif	//__SERVICEROUTERAPP_H__
