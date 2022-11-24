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

#ifndef __STATESERVICEAPP_H__
#define __STATESERVICEAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>
#include <cmsLib/redis/RedisProtoBufThreadCache.h>
#include <cmsLib/lua/ScriptContext.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/LinkToHolder.h>

#include "config/StateConfig.h"

#include "network/DataRouterLinkTo.h"

#include "states/StateService.h"

class StateServiceApp : public ServerAppBase, public IEurekaClientIntegrate
{
private:
	StateServiceApp();

public:
	static StateServiceApp& getInstance();
	virtual ~StateServiceApp();

	StateConfig* get_config();

	virtual void main_loop();

public:
	void send_to_gate( SProtocolHead& head, BasicProtocol* pro);
	void send_to_datarouter(NetProtocol* pro);

	boost::thread_specific_ptr<RedisClient>& get_redisclient_thread() { return this->redis_inthread_; }
	RedisClient* get_redisclient() { return redis_inthread_.get(); }
	boost::thread_specific_ptr<RedisProtoBufThreadCache>& get_rpcache_thread() { return this->rpcache_inthread_; }
	RedisProtoBufThreadCache* get_redisprotocache() { return rpcache_inthread_.get(); }

	StateService* get_next_dispatcher();

public:

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };

	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE, 
		std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);
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

	StateConfig* load_stateconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void online_user_maintance_timer(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool								is_ready_;

	//state service
	boost::scoped_array<StateService>	all_states_;
	volatile S_INT_32					cur_state_index_;

	//run in main thread
	LinkToHolder<DataRouterLinkTo>		datarouter_link_mth_;

	//network
	boost::thread_specific_ptr<RedisClient>					redis_inthread_;
	boost::thread_specific_ptr<RedisProtoBufThreadCache>	rpcache_inthread_;

	boost::scoped_ptr<StateConfig>				conf_;

public:
	void on_disconnected_with_datarouter(DataRouterLinkTo* plink);
	void on_datarouter_regist_result(DataRouterLinkTo* plink);
};

#define svrApp (StateServiceApp::getInstance())

#endif	//__STATESERVICEAPP_H__
