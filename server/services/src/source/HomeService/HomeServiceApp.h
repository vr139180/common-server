#ifndef __HOMESERVICEAPP_H__
#define __HOMESERVICEAPP_H__

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

#include "config/HomeConfig.h"

#include "network/FightRouterLinkTo.h"
#include "network/DataRouterLinkTo.h"

#include "lobby/LobbyUser.h"
#include "lobby/LobbyService.h"

class HomeServiceApp : public ServerAppBase, public IEurekaClientIntegrate
{
private:
	HomeServiceApp();

public:
	static HomeServiceApp& getInstance();
	virtual ~HomeServiceApp();

	HomeConfig* get_config();

	virtual void main_loop();

public:
	void send_protocol_to_res(BasicProtocol* pro);
	void send_protocol_to_gate(BasicProtocol* pro);

	void send_protocol_to_fightrouter(PRO::ERK_SERVICETYPE to, BasicProtocol* pro);

	boost::thread_specific_ptr<RedisClient>& get_redisclient_thread() { return this->redis_inthread_; }
	RedisClient* get_redisclient() { return redis_inthread_.get(); }
	boost::thread_specific_ptr<RedisProtoBufThreadCache>& get_rpcache_thread() { return this->rpcache_inthread_; }
	RedisProtoBufThreadCache* get_redisprotocache() { return rpcache_inthread_.get(); }

	boost::thread_specific_ptr<ScriptContext>& get_luacontext_thread() { return this->lua_inthread_; }
	ScriptContext* get_luacontext() { return lua_inthread_.get(); }

public:

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };

	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE, 
		std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);

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

	HomeConfig* load_homeconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool								is_ready_;

	//lobby service
	boost::scoped_array<LobbyService>	all_lobbys_;

	//run in main thread
	LinkToHolder<DataRouterLinkTo>		datarouter_link_mth_;
	//fight router
	LinkToHolder<FightRouterLinkTo>		fightrouter_link_mth_;

	//network
	boost::thread_specific_ptr<RedisClient>					redis_inthread_;
	boost::thread_specific_ptr<RedisProtoBufThreadCache>	rpcache_inthread_;

	boost::thread_specific_ptr<ScriptContext>	lua_inthread_;

	boost::scoped_ptr<HomeConfig>				conf_;

public:
	void on_disconnected_with_datarouter(DataRouterLinkTo* plink);
	void on_datarouter_regist_result(DataRouterLinkTo* plink);

	void on_disconnected_with_fightrouterservice(FightRouterLinkTo* plink);
	void on_fightrouterservice_regist_result(FightRouterLinkTo* plink);

};

#define svrApp (HomeServiceApp::getInstance())

#endif	//__HOMESERVICEAPP_H__
