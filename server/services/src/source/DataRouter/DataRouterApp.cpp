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

#include "DataRouterApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/global_const.h>

USE_PROTOCOL_NAMESPACE

DataRouterApp& DataRouterApp::getInstance()
{
	static DataRouterApp s_instance_;
	return s_instance_;
}

DataRouterApp::DataRouterApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,is_ready_(false)
{
}

DataRouterApp::~DataRouterApp()
{
}

bool DataRouterApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER))
	{
		logFatal(out_runtime, "DataRouter load svr config file failed");
		return false;
	}

	RouterConfig* cf = load_routerconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "DataRouter load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

RouterConfig* DataRouterApp::load_routerconfig()
{
	RouterConfig* config = new RouterConfig();
	std::unique_ptr<RouterConfig> xptr(config);

	std::string fstr = "/system/datarouter_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);

	return xptr.release();
}

#include <cmsLib/net/NetHashingWithVNode.h>

bool DataRouterApp::pre_init()
{
	/*
	NetHashingWithVNode<S_INT_64> nodes;
	nodes.init_vnode(800);
	nodes.add_realnode(10, 10);
	nodes.add_realnode(100, 100);
	nodes.add_realnode(50, 50);
	nodes.add_realnode(80, 80);
	nodes.build_nethashing();

	S_INT_64 val = nodes.get_netnode_byval(100);
	logDebug(out_runtime, "%lld", val);
	val = nodes.get_netnode_byval(1000);
	logDebug(out_runtime, "%lld", val);
	val = nodes.get_netnode_byval(5);
	logDebug(out_runtime, "%lld", val);
	val = nodes.get_netnode_byval(50);
	logDebug(out_runtime, "%lld", val);
	*/

	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	gate_links_from_.init_holder();
	fightrouter_links_from_.init_holder();
	servicerouter_links_from_.init_holder();
	state_links_from_.init_holder();

	home_links_from_.init_holder(VNODE_HOMESERVICE_NUM);

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	std::list<NETSERVICE_TYPE> router_types;
	router_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_HOME);

	EurekaNodeInfo enode;
	if (!EurekaClusterClient::get_eureka_masterinfo(gopt.eureka_.c_str(), enode))
	{
		logError(out_runtime, "xxxxxxxxxx-- access url:%s get eureka master failed......", gopt.eureka_.c_str());
		return false;
	}

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER, cf.get_ip().c_str(), cf.get_port(), 
		EurekaServerExtParam(), enode, subscribe_types, router_types, true);

	return true;
}

bool DataRouterApp::init_network()
{
    int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("DataRouter init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("DataRouter init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool DataRouterApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	if (acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
	{
		logInfo(out_runtime, ("<<<<<<<<<<<<DataRouter listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
	}
	else
	{
		logFatal(out_runtime, ("<<<<<<<<<<<<DataRouter listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
	}

	std::string verfmt = ShareUtil::str_format<128>(
		"DataRouter VER:%s SVN:%s PID:%d Listen On PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

	OSSystem::mOS->SetAppTitle(verfmt.c_str());

	return true;
}

void DataRouterApp::uninit_network()
{
	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	gate_links_from_.uninit_holder();
	fightrouter_links_from_.uninit_holder();
	servicerouter_links_from_.uninit_holder();

	home_links_from_.uninit_holder();
	state_links_from_.uninit_holder();

	session_from_.unint_sessions();

	EurekaClusterClient::instance().uninit();
}

void DataRouterApp::uninit()
{
	acceptor_.reset();
}

void DataRouterApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &DataRouterApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&DataRouterApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void DataRouterApp::main_loop()
{
	OSSystem::mOS->UpdateNowTick();
	int loopnum = conf_->loopnum_;

	int sleepstep =0;
	while( 1)
	{
		if( sleepstep >= loopnum)
		{
			OSSystem::mOS->thread_yield();
			sleepstep =0;
		}
		++sleepstep;

		if( will_quit_app_) break;

		//update time
		OSSystem::mOS->UpdateNowTick();

		app_timer_.timer_tick();

		CommandBase *pCmd =pop_one_cmd();
		std::unique_ptr<CommandBase> a_pcmd( pCmd);

		if( pCmd == 0)
		{
			sleepstep =loopnum;
			continue;
		}

		pCmd->run();
	}
}

NetAcceptorEvent::NetSessionPtr DataRouterApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void DataRouterApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	RouterSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_runtime, "me(DataRouter) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_runtime, "me(DataRouter) listen a connected request, and create a connection successfully");
	}
}

void DataRouterApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
}

void DataRouterApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void DataRouterApp::on_connection_timeout(RouterSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "DataRouter listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void DataRouterApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		gate_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void DataRouterApp::on_disconnected_with_homeservice(HomeServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		home_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void DataRouterApp::on_disconnected_with_stateservice(StateServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		state_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void DataRouterApp::on_disconnected_with_fightrouter(FightRouterLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		fightrouter_links_from_.return_freelink(plink);
		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void DataRouterApp::on_disconnected_with_servicerouter(ServiceRouterLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		servicerouter_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void DataRouterApp::dispatch_to_router(NetProtocol* pro)
{
	logDebug(out_runtime, "msg router from:%s to:%s msgid:%d",
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.from_type_).c_str(),
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.to_type_).c_str(),
		pro->get_msg());

	std::unique_ptr<NetProtocol> xptr(pro);
	NETSERVICE_TYPE gto = (NETSERVICE_TYPE)pro->get_to();
	if (gto == NETSERVICE_TYPE::ERK_SERVICE_GATE) {
		router_to_gate(xptr.release());
	}
	else if (gto == NETSERVICE_TYPE::ERK_SERVICE_HOME) {
		if (pro->circle_out(VNODE_MAX_CIRCLES))
			return;

		router_to_home(xptr.release());
	}
	else if (gto == NETSERVICE_TYPE::ERK_SERVICE_STATE) {
		router_to_state(xptr.release());
	}
	else if (gto == NETSERVICE_TYPE::ERK_SERVICE_GAME || gto == NETSERVICE_TYPE::ERK_SERVICE_MATCHMAKING) {
		router_to_fightrouter(xptr.release());
	}
	else if (gto == NETSERVICE_TYPE::ERK_SERVICE_CHAT || gto == NETSERVICE_TYPE::ERK_SERVICE_FRIEND ||
		gto == NETSERVICE_TYPE::ERK_SERVICE_MAIL || gto == NETSERVICE_TYPE::ERK_SERVICE_UNION)
	{
		router_to_servicerouter(xptr.release());
	}
}

void DataRouterApp::router_to_state(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> ptr(pro);

	StateServiceLinkFrom* plink = state_links_from_.get_servicelink_random();
	if (plink)
		plink->send_protocol(ptr.release());
}

void DataRouterApp::router_to_home(NetProtocol* pro)
{
	home_links_from_.send_protocol(pro->get_useriid(), pro);
}

void DataRouterApp::router_to_gate(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> ptr(pro);

	S_INT_64 gateid = pro->head_.get_token_gateiid();
	GateServiceLinkFrom* plink = gate_links_from_.get_servicelink_byiid(gateid);
	if (plink)
		plink->send_protocol(ptr.release());
}

void DataRouterApp::router_to_fightrouter(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> ptr(pro);
	FightRouterLinkFrom* plink = fightrouter_links_from_.get_servicelink_random();
	if (plink)
		plink->send_protocol(ptr.release());
}

void DataRouterApp::router_to_servicerouter(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> ptr(pro);
	ServiceRouterLinkFrom* plink = servicerouter_links_from_.get_servicelink_random();
	if (plink)
		plink->send_protocol(ptr.release());
}