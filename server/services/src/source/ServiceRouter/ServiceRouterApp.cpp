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

#include "ServiceRouterApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>

USE_PROTOCOL_NAMESPACE

ServiceRouterApp& ServiceRouterApp::getInstance()
{
	static ServiceRouterApp s_instance_;
	return s_instance_;
}

ServiceRouterApp::ServiceRouterApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,is_ready_(false)
{
}

ServiceRouterApp::~ServiceRouterApp()
{
}

bool ServiceRouterApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER))
	{
		logFatal(out_runtime, "ServiceRouter load svr config file failed");
		return false;
	}

	RouterConfig* cf = load_routerconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "ServiceRouter load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

RouterConfig* ServiceRouterApp::load_routerconfig()
{
	RouterConfig* config = new RouterConfig();
	std::unique_ptr<RouterConfig> xptr(config);

	std::string fstr = "/system/servicerouter_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	config->redis_.load_from_xml(rds);

	return xptr.release();
}

bool ServiceRouterApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	gate_links_from_.init_holder();

	chat_links_from_.init_holder(800);
	mail_links_from_.init_holder(800);
	friend_links_from_.init_holder(800);

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER);

	std::list< NETSERVICE_TYPE> router_types;
	router_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_MAIL);
	router_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_FRIEND);
	router_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_CHAT);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER, cf.get_ip().c_str(), cf.get_port(), 
		EurekaServerExtParam(), gopt.eip.c_str(), gopt.eport, subscribe_types, router_types, true);

	return true;
}

bool ServiceRouterApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("ServiceRouter init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("ServiceRouter init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool ServiceRouterApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	if (acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
	{
		logInfo(out_runtime, ("<<<<<<<<<<<<ServiceRouter listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
	}
	else
	{
		logFatal(out_runtime, ("<<<<<<<<<<<<ServiceRouter listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
	}

    char app_title_[200];
    sprintf(app_title_, "ServiceRouter VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void ServiceRouterApp::uninit_network()
{
	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	gate_links_from_.uninit_holder();
	chat_links_from_.uninit_holder();
	mail_links_from_.uninit_holder();
	friend_links_from_.uninit_holder();

	session_from_.unint_sessions();

	EurekaClusterClient::instance().uninit();
}

void ServiceRouterApp::uninit()
{
	acceptor_.reset();
}

void ServiceRouterApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &ServiceRouterApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&ServiceRouterApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void ServiceRouterApp::main_loop()
{
	OSSystem::mOS->UpdateNowTick();
	u64 st =OSSystem::mOS->GetTicks();
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

NetAcceptorEvent::NetSessionPtr ServiceRouterApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void ServiceRouterApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
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
		logError(out_runtime, "me(ServiceRouter) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_runtime, "me(ServiceRouter) listen a connected request, and create a connection successfully");
	}
}

void ServiceRouterApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
}

void ServiceRouterApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void ServiceRouterApp::on_connection_timeout(RouterSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "ServiceRouter listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void ServiceRouterApp::on_datarouter_regist_result(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_regist_result(plink);
}

void ServiceRouterApp::on_disconnected_with_datarouter(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_disconnected(plink);
}

void ServiceRouterApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
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

void ServiceRouterApp::on_disconnected_with_chatservice(ChatServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		chat_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();

	}
}

void ServiceRouterApp::send_protocal_to_gate(S_INT_64 gateiid, BasicProtocol* msg)
{
	GateServiceLinkFrom* plink = gate_links_from_.get_servicelink_byiid(gateiid);
	if (plink == 0)
	{
		delete msg;
		return;
	}

	plink->send_netprotocol(msg);
}

void ServiceRouterApp::send_protocal_to_chat(S_INT_64 chathash, BasicProtocol* msg)
{
	chat_links_from_.send_protocol(chathash, msg);
}

void ServiceRouterApp::send_protocal_to_mail(S_INT_64 mailhash, BasicProtocol* msg)
{
	mail_links_from_.send_protocol(mailhash, msg);
}

void ServiceRouterApp::send_protocal_to_mail_circle(BasicProtocol* msg)
{
	//mail_links_from_.send_mth_protocol_circle(msg);
}

void ServiceRouterApp::send_protocal_to_friend(S_INT_64 frdhash, BasicProtocol* msg)
{
	friend_links_from_.send_protocol(frdhash, msg);
}

void ServiceRouterApp::on_disconnected_with_mailservice(MailServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		mail_links_from_.return_freelink(plink);

		plink->reset();
		psession->reset();

		session_from_.return_freesession_mth(psession);
	}
}

void ServiceRouterApp::on_disconnected_with_frdservice(FriendServiceLinkFrom* plink)
{
	RouterSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		friend_links_from_.return_freelink(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}
