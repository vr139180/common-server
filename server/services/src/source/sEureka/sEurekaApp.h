#ifndef __SEUREKAAPP_H__
#define __SEUREKAAPP_H__

#include <vector>
#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/tss.hpp>

#include <cmsLib/ServerAppBase.h>
#include <cmsLib/net/NetAcceptor.h>
#include <cmsLib/net/NetAcceptorEvent.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/EurekaConfig.h>
#include <gameLib/commons/SessionMthHolder.h>

#include "cluster/EurekaClusterCtrl.h"
#include "services/ServiceRegisterCtrl.h"

#include "EurekaService.h"

USE_PROTOCOL_NAMESPACE

class sEurekaApp : public ServerAppBase, public NetAcceptorEvent
{
	friend class EurekaClusterCtrl;
	friend class ServiceRegisterCtrl;
private:
	sEurekaApp();

public:
	static sEurekaApp& getInstance();
	virtual ~sEurekaApp();

	virtual void main_loop();
	//disable this funcion. system command be used by mainthread, and net/user command be used by eurekaservice thread
	virtual CommandBase* pop_one_cmd() { return 0; }
	virtual CommandBase* pop_net_cmd();

public:
	EurekaClusterCtrl* get_eurekactrl() { return &eureka_ctrl_; }
	ServiceRegisterCtrl* get_servicectrl() { return &service_ctrl_; }

	//redisclient threadlocal storage
	boost::thread_specific_ptr<RedisClient>& get_redisclient_thread() { return this->redis_inthread_; }
	RedisClient* get_redisclient() { return redis_inthread_.get(); }

protected:
	void remove_waitsession_no_mutex(EurekaSession* psession);
	void return_freesession_no_mutext(EurekaSession* psession);

public:

	//------------------------------implement NetAcceptorEvent ------------------------------//
	virtual NetAcceptorEvent::NetSessionPtr ask_free_netsession();
	virtual void accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err);

protected:
	virtual bool load_config();
	void load_redisscript();
	virtual bool pre_init();
	virtual bool init_network();
	virtual void uninit_network();
	virtual void uninit();

	virtual bool init_finish();

	virtual void register_timer();

protected:
	//timer,启动初始化过程
	void init_eureka_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish);

protected:
	//network
	std::shared_ptr<NetAcceptor>			acceptor_;

	SessionMthHolder<EurekaSession>			session_from_;

	boost::scoped_array<EurekaService>		services_;
	int										service_threads_num_;

	EurekaClusterCtrl						eureka_ctrl_;
	ServiceRegisterCtrl						service_ctrl_;

	//主线程使用的redis
	RedisClient								redis_;
	boost::thread_specific_ptr<RedisClient>	redis_inthread_;

	//eureka config
	boost::scoped_ptr<EurekaConfig>			conf_;

public:
	void on_connection_timeout( EurekaSession* session);

};

#define svrApp (sEurekaApp::getInstance())

#endif	//__SEUREKAAPP_H__
