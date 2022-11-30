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

#ifndef __FIGHTROUTERAPP_H__
#define __FIGHTROUTERAPP_H__

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

#include "config/FightRouterConfig.h"

#include "network/FightRouterSession.h"
#include "network/GameServiceLinkFrom.h"
#include "network/MatchMakingServiceLinkFrom.h"
#include "network/DataRouterLinkTo.h"
#include "network/GateServiceLinkFrom.h"
#include "network/GameRegionResolver.h"

class FightRouterApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	FightRouterApp();

public:
	static FightRouterApp& getInstance();
	virtual ~FightRouterApp();

	virtual void main_loop();

	FightRouterConfig* get_config() { return conf_.get(); }

public:
	void router_to_game(NetProtocol* pro);
	void router_to_game_from_othsvr(NetProtocol* pro);
	void router_to_gate(NetProtocol* pro);
	void router_to_home(NetProtocol* pro);

	void do_gameservice_bind_region(GameServiceLinkFrom* plink, S_INT_32 regionid);

public:

	//------------------------------implement NetAcceptorEvent ------------------------------//
	virtual NetAcceptorEvent::NetSessionPtr ask_free_netsession();
	virtual void accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err);

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };
	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE, 
		std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);
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

	FightRouterConfig* load_routerconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool										is_ready_;

	//network
	std::shared_ptr<NetAcceptor>				acceptor_;
	SessionMthHolder<FightRouterSession>		session_from_;

	LinkToHolder<DataRouterLinkTo>				datarouter_link_mth_;

	LinkFromHolder<MatchMakingServiceLinkFrom>	matchmaking_links_from_;
	LinkFromHolder<GateServiceLinkFrom>			gate_links_from_;
	GameRegionResolver							game_links_from_;

	boost::scoped_ptr<FightRouterConfig>		conf_;

public:
	void on_connection_timeout(FightRouterSession* session);

	void on_mth_servicebindservice_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_mth_gameregion_regist_ntf(NetProtocol* pro, bool& autorelease);

	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);
	void on_disconnected_with_gameservice(GameServiceLinkFrom* plink);
	void on_disconnected_with_matchmakingservice(MatchMakingServiceLinkFrom* plink);

	void on_disconnected_with_datarouter(DataRouterLinkTo* plink);
	void on_datarouter_regist_result(DataRouterLinkTo* plink);
};

#define svrApp (FightRouterApp::getInstance())

#endif	//__FIGHTROUTERAPP_H__
