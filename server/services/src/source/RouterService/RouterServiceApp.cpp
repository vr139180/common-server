#include "RouterServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>

USE_PROTOCOL_NAMESPACE

RouterServiceApp& RouterServiceApp::getInstance()
{
	static RouterServiceApp s_instance_;
	return s_instance_;
}

RouterServiceApp::RouterServiceApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,is_ready_(false)
{
}

RouterServiceApp::~RouterServiceApp()
{
}

bool RouterServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_ROUTER))
	{
		logFatal(out_boot, "RouterService load svr config file failed");
		return false;
	}

	RouterConfig* cf = load_routerconfig();
	if (cf == 0)
	{
		logFatal(out_boot, "RouterService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

RouterConfig* RouterServiceApp::load_routerconfig()
{
	RouterConfig* config = new RouterConfig();
	std::unique_ptr<RouterConfig> xptr(config);

	std::string fstr = "/system/router_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	tinyxml2::XMLElement* ch = root->FirstChildElement("chathash");
	if (ch == 0)
		return 0;
	config->chathash_plot_.chatmax = XmlUtil::GetXmlAttrInt(ch, "chatmax", 1);

	ch = root->FirstChildElement("mailhash");
	if (ch == 0)
		return 0;
	config->mailhash_plot_.mailmax = XmlUtil::GetXmlAttrInt(ch, "mailmax", 1);

	ch = root->FirstChildElement("friendhash");
	if (ch == 0)
		return 0;
	config->friendhash_plot_.frdmax = XmlUtil::GetXmlAttrInt(ch, "frdmax", 1);

	tinyxml2::XMLElement* rds = root->FirstChildElement("redis");
	if (rds == 0)
		return 0;

	config->redis_.load_from_xml(rds);

	return xptr.release();
}

bool RouterServiceApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);
	gate_links_from_.init_holder();
	chat_links_from_.init_holder();
	mail_links_from_.init_holder();
	friend_links_from_.init_holder();

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_ROUTER,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(), gopt.eip.c_str(), gopt.eport, subscribe_types);

	return true;
}

bool RouterServiceApp::init_network()
{
    int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator(500);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_boot, ("RouterService init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_boot, ("RouterService init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool RouterServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	if (acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
	{
		logInfo(out_boot, ("RouterService listen socket at %s:%d \n"), cf.get_ip().c_str(), cf.get_port());
	}
	else
	{
		logFatal(out_boot, ("RouterService listen socket at %s:%d failed\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
	}

    char app_title_[200];
    sprintf(app_title_, "RouterService VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void RouterServiceApp::uninit_network()
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

void RouterServiceApp::uninit()
{
	acceptor_.reset();
}

void RouterServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &RouterServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&RouterServiceApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void RouterServiceApp::main_loop()
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

NetAcceptorEvent::NetSessionPtr RouterServiceApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void RouterServiceApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
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
		logError(out_boot, "me(RouterService) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_net, "me(RouterService) listen a connected request, and create a connection successfully");
	}
}

void RouterServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
}

void RouterServiceApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void RouterServiceApp::on_connection_timeout(RouterSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_boot, "RouterService listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void RouterServiceApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
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

void RouterServiceApp::on_disconnected_with_chatservice(ChatServiceLinkFrom* plink)
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

void RouterServiceApp::send_protocal_to_gate(S_INT_64 gateiid, BasicProtocol* msg)
{
	GateServiceLinkFrom* plink = gate_links_from_.get_servicelink_byiid(gateiid);
	if (plink == 0)
	{
		delete msg;
		return;
	}

	plink->send_protocol(msg);
}

void RouterServiceApp::send_protocal_to_chat(int chathash, BasicProtocol* msg)
{
	chat_links_from_.send_mth_protocol(chathash, msg);
}

void RouterServiceApp::send_protocal_to_mail(int mailhash, BasicProtocol* msg)
{
	mail_links_from_.send_mth_protocol(mailhash, msg);
}

void RouterServiceApp::send_protocal_to_mail_circle(BasicProtocol* msg)
{
	mail_links_from_.send_mth_protocol_circle(msg);
}

void RouterServiceApp::send_protocal_to_friend(int frdhash, BasicProtocol* msg)
{
	friend_links_from_.send_mth_protocol(frdhash, msg);
}

void RouterServiceApp::on_disconnected_with_mailservice(MailServiceLinkFrom* plink)
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

void RouterServiceApp::on_disconnected_with_frdservice(FriendServiceLinkFrom* plink)
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

void RouterServiceApp::on_disconnected_with_homeservice(HomeServiceLinkFrom* plink)
{

}



