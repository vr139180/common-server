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
#include <gameLib/commons/SessionMthHolder.h>
#include <gameLib/commons/LinkFromHolder.h>
#include <gameLib/commons/LinkToHolder.h>
#include <gameLib/gatehome/GHUsersSeparate.h>

#include "config/HomeConfig.h"

#include "network/HomeSession.h"
#include "network/GateServiceLinkFrom.h"
#include "network/UnionClusterLinkTo.h"
#include "network/ResClusterLinkTo.h"

#include "lobby/LobbyUser.h"
#include "lobby/LobbyService.h"

class HomeServiceApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	HomeServiceApp();

public:
	static HomeServiceApp& getInstance();
	virtual ~HomeServiceApp();

	HomeConfig* get_config() { return conf_.get(); }

	virtual void main_loop();

public:
	SessionMthHolder<HomeSession>* get_session_from() { return &session_from_; }
	void send_protocol_to_res(BasicProtocol* pro);
	void send_protocol_to_gate(BasicProtocol* pro);

	boost::thread_specific_ptr<RedisClient>& get_redisclient_thread() { return this->redis_inthread_; }
	RedisClient* get_redisclient() { return redis_inthread_.get(); }
	boost::thread_specific_ptr<RedisProtoBufThreadCache>& get_rpcache_thread() { return this->rpcache_inthread_; }
	RedisProtoBufThreadCache* get_redisprotocache() { return rpcache_inthread_.get(); }

	boost::thread_specific_ptr<ScriptContext>& get_luacontext_thread() { return this->lua_inthread_; }
	ScriptContext* get_luacontext() { return lua_inthread_.get(); }

	LobbyService* get_lobbysvr_by_slot(int slot);
	void post_syscmd_2_lobbyservice(S_INT_64 token, CommandBase* pcmd);

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
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool								is_ready_;

	//lobby users placeholder
	GHUsersSeparate<LobbyUser>			all_lobby_users_;
	//lobby service
	boost::scoped_array<LobbyService>	all_lobbys_;
	int									lobby_nums_;

	//gate
	LinkFromHolder<GateServiceLinkFrom>		gate_link_map_;
	GateServiceLinkFrom*					gate_link_;
	//run in main thread
	LinkToHolder<ResClusterLinkTo>		res_link_mth_;

	//network
	std::shared_ptr<NetAcceptor>			acceptor_;
	SessionMthHolder<HomeSession>		session_from_;

	boost::thread_specific_ptr<RedisClient>	redis_inthread_;
	boost::thread_specific_ptr<RedisProtoBufThreadCache> rpcache_inthread_;

	boost::thread_specific_ptr<ScriptContext>	lua_inthread_;

	boost::scoped_ptr<HomeConfig>		conf_;

public:
	void on_connection_timeout(HomeSession* session);

	void on_disconnected_with_resservice(ResClusterLinkTo* plink);
	void on_resservice_regist_result( ResClusterLinkTo* plink);

	void on_disconnected_with_unionservice(UnionClusterLinkTo* plink);
	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);

	void on_mth_servicebindservice_req(BasicProtocol* pro, bool& autorelease, void* session);
	void on_mth_gatebindhome_confirm(BasicProtocol* message, bool& autorelease);

	void on_mth_userproxyslot_req(BasicProtocol* message, bool& autorelease);
};

#define svrApp (HomeServiceApp::getInstance())

#endif	//__HOMESERVICEAPP_H__
