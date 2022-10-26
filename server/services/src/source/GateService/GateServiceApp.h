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

#include "config/GateConfig.h"

#include "network/DataRouterLinkTo.h"
#include "network/ServiceRouterLinkTo.h"
#include "network/FightRouterLinkTo.h"

#include "player/GamePlayer.h"

class GateServiceApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	GateServiceApp();

public:
	static GateServiceApp& getInstance();
	virtual ~GateServiceApp();

	virtual void main_loop();

public:
	void route_to_datarouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro);
	void send_to_datarouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg);

	void route_to_servicerouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro);
	void send_to_servicerouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg);

	void route_to_fightrouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro);
	void send_to_fightrouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg);

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

protected:
	//network
	std::shared_ptr<NetAcceptor>	acceptor_;

	LinkToHolder<DataRouterLinkTo>		datarouter_link_mth_;
	LinkToHolder<ServiceRouterLinkTo>	svrrouter_link_mth_;
	LinkToHolder<FightRouterLinkTo>		fightrouter_link_mth_;

	boost::scoped_ptr<GateConfig>	conf_;
public:
	void on_disconnected_with_datarouter(DataRouterLinkTo* plink);
	void on_datarouter_regist_result(DataRouterLinkTo* plink);

	void on_disconnected_with_svrrouter(ServiceRouterLinkTo* plink);
	void on_svrrouter_regist_result(ServiceRouterLinkTo* plink);

	void on_disconnected_with_fightrouter(FightRouterLinkTo* plink);
	void on_fightrouter_regist_result(FightRouterLinkTo* plink);

public:
	
};

#define svrApp (GateServiceApp::getInstance())

#endif	//__GATESERVICEAPP_H__
