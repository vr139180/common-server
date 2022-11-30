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

#include "FightRouterApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <worldsLib/GameWorldHelper.h>

USE_PROTOCOL_NAMESPACE

FightRouterApp& FightRouterApp::getInstance()
{
	static FightRouterApp s_instance_;
	return s_instance_;
}

FightRouterApp::FightRouterApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,is_ready_(false)
{
}

FightRouterApp::~FightRouterApp()
{
}

bool FightRouterApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER))
	{
		logFatal(out_runtime, "FightRouter load svr config file failed");
		return false;
	}

	FightRouterConfig* cf = load_routerconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "FightRouter load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	if (!GameWorldHelper::instance().init_gameworld())
	{
		logFatal(out_runtime, "FightRouter load worldregion.xml file failed");
		return false;
	}

	return true;
}

FightRouterConfig* FightRouterApp::load_routerconfig()
{
	FightRouterConfig* config = new FightRouterConfig();
	std::unique_ptr<FightRouterConfig> xptr(config);

	std::string fstr = "/system/fightrouter_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	tinyxml2::XMLElement* rds = root->FirstChildElement("redis");
	if (rds == 0)
		return 0;

	config->redis_.load_from_xml(rds);

	return xptr.release();
}

bool FightRouterApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	gate_links_from_.init_holder();
	matchmaking_links_from_.init_holder();
	game_links_from_.init_holder();

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER);

	std::list< NETSERVICE_TYPE> router_types;
	//router_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_GAME);

	EurekaNodeInfo enode;
	if (!EurekaClusterClient::get_eureka_masterinfo(gopt.eureka_.c_str(), enode))
	{
		logError(out_runtime, "xxxxxxxxxx-- access url:%s get eureka master failed......", gopt.eureka_.c_str());
		return false;
	}

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER, cf.get_ip().c_str(), cf.get_port(), 
		EurekaServerExtParam(), enode, subscribe_types, router_types, true);

	return true;
}

bool FightRouterApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("FightRouter init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("FightRouter init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool FightRouterApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	if (acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
	{
		logInfo(out_runtime, ("<<<<<<<<<<<<FightRouter listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
	}
	else
	{
		logFatal(out_runtime, ("<<<<<<<<<<<<FightRouter listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
	}

    char app_title_[200];
    sprintf(app_title_, "FightRouter VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void FightRouterApp::uninit_network()
{
	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	gate_links_from_.uninit_holder();
	game_links_from_.uninit_holder();
	matchmaking_links_from_.uninit_holder();
	datarouter_link_mth_.free_all();

	session_from_.unint_sessions();

	EurekaClusterClient::instance().uninit();
}

void FightRouterApp::uninit()
{
	acceptor_.reset();
}

void FightRouterApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &FightRouterApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&FightRouterApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void FightRouterApp::main_loop()
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

NetAcceptorEvent::NetSessionPtr FightRouterApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void FightRouterApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	FightRouterSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_runtime, "me(FightRouter) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_runtime, "me(FightRouter) listen a connected request, and create a connection successfully");
	}
}

void FightRouterApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	datarouter_link_mth_.connect_to();
}

void FightRouterApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void FightRouterApp::on_connection_timeout(FightRouterSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "FightRouter listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void FightRouterApp::on_disconnected_with_gameservice(GameServiceLinkFrom* plink)
{
	FightRouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		game_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void FightRouterApp::on_disconnected_with_matchmakingservice(MatchMakingServiceLinkFrom* plink)
{
	FightRouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		matchmaking_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void FightRouterApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
{
	FightRouterSession* psession = plink->get_session();
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

void FightRouterApp::on_datarouter_regist_result(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_regist_result(plink);
}

void FightRouterApp::on_disconnected_with_datarouter(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_disconnected(plink);
}

void FightRouterApp::do_gameservice_bind_region(GameServiceLinkFrom* plink, S_INT_32 regionid)
{
	game_links_from_.bind_to_region(plink, regionid);
}

void FightRouterApp::router_to_game(NetProtocol* pro)
{
	logDebug(out_runtime, "msg router from:%s to:%s msgid:%d",
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.from_type_).c_str(),
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.to_type_).c_str(),
		pro->get_msg());

	game_links_from_.dispath_to_game(pro);
}

void FightRouterApp::router_to_game_from_othsvr(NetProtocol* pro)
{
	logDebug(out_runtime, "msg router from:%s to:%s msgid:%d",
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.from_type_).c_str(),
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.to_type_).c_str(),
		pro->get_msg());

	game_links_from_.dispatch_to_game_from_othsvr(pro);
}

void FightRouterApp::router_to_gate(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> ptr(pro);

	logDebug(out_runtime, "msg router from:%s to:%s msgid:%d",
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.from_type_).c_str(),
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.to_type_).c_str(),
		pro->get_msg());

	S_INT_64 gateid = pro->head_.get_token_gateiid();
	GateServiceLinkFrom* plink = gate_links_from_.get_servicelink_byiid(gateid);
	if (plink)
		plink->send_protocol(ptr.release());
}

void FightRouterApp::router_to_home(NetProtocol* pro)
{
	logDebug(out_runtime, "msg router from:%s to:%s msgid:%d",
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.from_type_).c_str(),
		NetServiceType::to_string((NETSERVICE_TYPE)pro->head_.to_type_).c_str(),
		pro->get_msg());

	datarouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_HOME, pro);
}
