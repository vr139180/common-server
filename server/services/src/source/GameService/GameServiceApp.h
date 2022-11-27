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

#ifndef __GAMESERVICEAPP_H__
#define __GAMESERVICEAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/LinkToHolder.h>
#include <gameLib/commons/SessionMthHolder.h>

#include "network/FightRouterLinkTo.h"
#include "region/RegionChannelService.h"

#include "config/GameConfig.h"

class GameServiceApp : public ServerAppBase, public IEurekaClientIntegrate
{
private:
	GameServiceApp();

public:
	static GameServiceApp& getInstance();
	virtual ~GameServiceApp();

	GameConfig* get_config();

	virtual void main_loop();

	void send_protocol_to_gate(BasicProtocol* pro);
	void send_protocol_to_home(BasicProtocol* pro);

public:

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };

	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE type, std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);
	virtual void mth_notify_routerbalance_new(NETSERVICE_TYPE, std::list<S_INT_64>& svrs) {}

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

	GameConfig* load_gameconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);

protected:
	bool							is_ready_;

	//network
	LinkToHolder<FightRouterLinkTo>				fightrouter_link_mth_;

	boost::scoped_array<RegionChannelService>	all_channels_;
	int											channel_num_;

	boost::scoped_ptr<GameConfig>	conf_;

public:
	void on_disconnected_with_fightrouter(FightRouterLinkTo* plink);
	void on_fightrouter_regist_result(FightRouterLinkTo* plink);
};

#define svrApp (GameServiceApp::getInstance())

#endif	//__GAMESERVICEAPP_H__
