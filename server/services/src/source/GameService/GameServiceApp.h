#ifndef __GAMESERVICEAPP_H__
#define __GAMESERVICEAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/SessionMthHolder.h>

#include "config/GameConfig.h"

#include "network/GameSession.h"
#include "network/GateServiceLinkFrom.h"
#include "network/HomeServiceLinkTo.h"

class GameServiceApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	GameServiceApp();

public:
	static GameServiceApp& getInstance();
	virtual ~GameServiceApp();

	virtual void main_loop();

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

	GameConfig* load_gameconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	bool	is_ready_;
	//network
	std::shared_ptr<NetAcceptor>	acceptor_;
	SessionMthHolder<GameSession>	session_from_;

	boost::scoped_ptr<GameConfig>	conf_;

public:
	void on_connection_timeout(GameSession* session);

	void on_disconnected_with_homeservice(HomeServiceLinkTo* plink);
	void on_homeservice_regist_result(HomeServiceLinkTo* plink);

	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);
};

#define svrApp (GameServiceApp::getInstance())

#endif	//__GAMESERVICEAPP_H__
