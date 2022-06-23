#ifndef __UNIONSERVICEAPP_H__
#define __UNIONSERVICEAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/SessionMthHolder.h>

#include "config/UnionConfig.h"

#include "network/UnionSession.h"
#include "network/HomeServiceLinkFrom.h"
#include "network/GateServiceLinkFrom.h"
#include "network/GameServiceLinkFrom.h"


class UnionServiceApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	UnionServiceApp();

public:
	static UnionServiceApp& getInstance();
	virtual ~UnionServiceApp();

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

	UnionConfig* load_unionconfig();

protected:
	//timer
	void auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//注册成功之后标注为true
	bool							is_ready_;

	//network
	std::shared_ptr<NetAcceptor>	acceptor_;
	SessionMthHolder<UnionSession>	session_from_;

	boost::scoped_ptr<UnionConfig>	conf_;

public:
	void on_connection_timeout(UnionSession* session);

	void on_disconnected_with_homeservice(HomeServiceLinkFrom* plink);
	void on_disconnected_with_gameservice(GameServiceLinkFrom* plink);
	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);
};

#define svrApp (UnionServiceApp::getInstance())

#endif	//__UNIONSERVICEAPP_H__
