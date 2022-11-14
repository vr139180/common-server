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
#include "network/EurekaLinkFrom.h"

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

	EurekaClusterCtrl						eureka_ctrl_;
	ServiceRegisterCtrl						service_ctrl_;

	//主线程使用的redis
	RedisClient								redis_;
	boost::thread_specific_ptr<RedisClient>	redis_inthread_;

	//eureka config
	boost::scoped_ptr<EurekaConfig>			conf_;

public:
	//启动成功
	void on_notify_boosted();

	void on_connection_timeout( EurekaSession* session);

	void on_disconnected_with_linkfrom(EurekaLinkFrom* plink);
};

#define svrApp (sEurekaApp::getInstance())

#endif	//__SEUREKAAPP_H__
