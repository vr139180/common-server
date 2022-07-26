#ifndef __GATESERVICEAPP_H__
#define __GATESERVICEAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/LinkToHolder.h>
#include <gameLib/commons/SessionMthHolder.h>

#include "config/GateConfig.h"

#include "network/GateSession.h"

#include "network/HomeServiceLinkTo.h"
#include "network/UnionClusterLinkTo.h"
#include "network/GameServiceLinkTo.h"
#include "network/RouterServiceLinkTo.h"

#include "player/GamePlayer.h"

typedef enum tagGateBindHomeStep {
	GateBindHome_Waiting,
	GateBindHome_CanAskBind,
	GateBindHome_AskBind,
	GateBindHome_Confirm,
	GateBindHome_Done
}GateBindHomeStep;

class GateServiceApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	GateServiceApp();

public:
	static GateServiceApp& getInstance();
	virtual ~GateServiceApp();

	virtual void main_loop();

public:
	HomeServiceLinkTo* get_homeholder() { return home_linkto_.get(); }
	void send_to_homeservice(BasicProtocol* msg) { home_linkto_->send_protocol(msg); }

	bool is_bindhome() { return bind_home_step == GateBindHomeStep::GateBindHome_Done; }

	void send_protocol_to_router(BasicProtocol* pro);
	
public:

	//------------------------------implement NetAcceptorEvent ------------------------------//
	virtual NetAcceptorEvent::NetSessionPtr ask_free_netsession();
	virtual void accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err);

	//------------------------------implement IEurekaClientIntegrate-------------------------//
	virtual ThreadLock& get_mth_threadlock() { return lock_; }
	virtual void regist_eurekacommand(CommandBase *p) { regist_syscmd(p); }
	virtual TimerKey add_apptimer_proxy(int step, APPTIMER_FUN_MAP f) { return add_apptimer(step, f); }
	virtual void del_apptimer_proxy(TimerKey tid) { del_apptimer(tid); };

	virtual void mth_notify_servicenode_new(NETSERVICE_TYPE type, 
		std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids);

	virtual void mth_service_registed(S_INT_64 sid);
	virtual void mth_eureka_losted();

	virtual void on_mth_gatebindhome_ack(bool succ, ServiceNodeInfo* pnode, S_INT_64 gateid, S_INT_64 bindtoken);

protected:
	virtual bool load_config();
	virtual bool pre_init();
	virtual bool init_network();
	virtual void uninit_network();
	virtual void uninit();

	virtual bool init_finish();

	virtual void register_timer();

	GateConfig* load_gateconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//link to home
	std::shared_ptr<HomeServiceLinkTo>	home_linkto_;
	GateBindHomeStep	bind_home_step;

	//network
	std::shared_ptr<NetAcceptor>	acceptor_;
	SessionMthHolder<GateSession>	session_from_;

	LinkToHolder<RouterServiceLinkTo>	router_link_mth_;

	boost::scoped_ptr<GateConfig>	conf_;
public:
	void on_connection_timeout(GateSession* session);

	void on_disconnected_with_homeservice(HomeServiceLinkTo* plink);
	void on_homeservice_regist_result( HomeServiceLinkTo* plink);

	void on_disconnected_with_routerservice(RouterServiceLinkTo* plink);
	void on_routerservice_regist_result( RouterServiceLinkTo* plink);

	void on_disconnected_with_unionservice(UnionClusterLinkTo* plink);
	void on_disconnected_with_gameservice(GameServiceLinkTo* plink);

public:
	void on_disconnected_with_player(GateSession* psession);
	void on_player_login(GateSession* psession);
};

#define svrApp (GateServiceApp::getInstance())

#endif	//__GATESERVICEAPP_H__
