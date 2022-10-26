#ifndef __DATAROUTERAPP_H__
#define __DATAROUTERAPP_H__

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/commons/SessionMthHolder.h>
#include <gameLib/commons/LinkFromHolder.h>

#include "config/RouterConfig.h"

#include "network/RouterSession.h"

#include "network/GateServiceLinkFrom.h"
#include "network/HomeServiceLinkFrom.h"
#include "network/StateServiceLinkFrom.h"

class DataRouterApp : public ServerAppBase, public NetAcceptorEvent, public IEurekaClientIntegrate
{
private:
	DataRouterApp();

public:
	static DataRouterApp& getInstance();
	virtual ~DataRouterApp();

	virtual void main_loop();

	RouterConfig* get_config() { return conf_.get(); }

public:
	void send_protocal_to_gate( S_INT_64 gateiid, BasicProtocol* msg);
	template<class T>
	void broad_protocal_to_gate(T* msg) {
		gate_links_from_.broadcast<T>(msg);
	}

	void send_protocal_to_chat(int chathash, BasicProtocol* msg);
	void send_protocal_to_mail(int mailhash, BasicProtocol* msg);
	void send_protocal_to_mail_circle( BasicProtocol* msg);
	template<class T>
	void broad_protocal_to_mails(T* msg) {
		mail_links_from_.broadcast<T>(msg);
	}

	void send_protocal_to_friend(int frdhash, BasicProtocol* msg);

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

	LinkFromHolder<GateServiceLinkFrom>	gate_links_from_;

	boost::scoped_ptr<RouterConfig>	conf_;

public:
	void on_connection_timeout(RouterSession* session);

	void on_mth_servicebindservice_req(BasicProtocol* pro, bool& autorelease, void* session);

	void on_disconnected_with_homeservice(HomeServiceLinkFrom* plink);
	void on_disconnected_with_gateservice(GateServiceLinkFrom* plink);
};

#define svrApp (DataRouterApp::getInstance())

#endif	//__DATAROUTERAPP_H__
