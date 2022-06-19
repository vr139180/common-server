#include "GateServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/global_const.h>

#include "player/GamePlayerCtrl.h"

USE_PROTOCOL_NAMESPACE

GateServiceApp& GateServiceApp::getInstance()
{
	static GateServiceApp s_instance_;
	return s_instance_;
}

GateServiceApp::GateServiceApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,bind_home_step(GateBindHome_Waiting)
{
}

GateServiceApp::~GateServiceApp()
{
}

bool GateServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_GATE))
	{
		logFatal(out_boot, "GateService load svr config file failed");
		return false;
	}

	GateConfig* cf = load_gateconfig();
	if (cf == 0)
	{
		logFatal(out_boot, "GateService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

GateConfig* GateServiceApp::load_gateconfig()
{
	GateConfig* config = new GateConfig();
	std::auto_ptr<GateConfig> xptr(config);

	std::string fstr = "/system/gate_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	return xptr.release();
}

bool GateServiceApp::pre_init()
{
	this->session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_ROUTER);

	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_UNION);
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_GAME);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_GATE,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		gopt.eip.c_str(), gopt.eport, subscribe_types);

	GamePlayerCtrl::instance().init_gameplayerctrl(GATEHOME_GROUP_NUM);

	return true;
}

bool GateServiceApp::init_network()
{
	int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator(500);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_boot, ("GateService init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_boot, ("GateService init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool GateServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	int maxsvr = cf.get_globaloption().svrnum_min;
    if( acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), maxsvr))
    {
		logInfo( out_boot, ("GateService listen socket at %s:%d \n"), cf.get_ip().c_str(), cf.get_port());
    }
    else
    {
		logFatal( out_boot, ("GateService listen socket at %s:%d failed\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
    }

	GamePlayerCtrl::instance().start();

    char app_title_[200];
    sprintf(app_title_, "GateService VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void GateServiceApp::uninit_network()
{
	GamePlayerCtrl::instance().stop();

	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	session_from_.unint_sessions();

	EurekaClusterClient::instance().uninit();
	GamePlayerCtrl::instance().uninit_gameplayerctrl();
}

void GateServiceApp::uninit()
{
	acceptor_.reset(0);
}

void GateServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &GateServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&GateServiceApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	//5秒维护一次 登陆超时
	this->add_apptimer(1000 * 5, boost::BOOST_BIND(&GamePlayerCtrl::maintnce_proxylogin_timer, &GamePlayerCtrl::instance(),
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void GateServiceApp::main_loop()
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
		std::auto_ptr<CommandBase> a_pcmd( pCmd);

		if( pCmd == 0)
		{
			sleepstep =loopnum;
			continue;
		}

		pCmd->run();
	}
}

NetAcceptorEvent::NetSessionPtr GateServiceApp::ask_free_netsession()
{
	/*
	if (is_bindhome() == false)
	{
		NetAcceptorEvent::NetSessionPtr new_session;
		return new_session;
	}
	*/

	//logDebug(out_net, "gate service listen a client connection request.....");

	ThreadLockWrapper guard(lock_);
	return session_from_.ask_free_netsession_mth();
}

void GateServiceApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	GateSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_boot, "me(GateService) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_net, "me(GateService) listen a connected request, and create a connection successfully");
	}
}

void GateServiceApp::send_protocol_to_router(BasicProtocol* pro)
{
	router_link_mth_.send_mth_protocol(pro);
}

void GateServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	//bind
	if (bind_home_step == GateBindHomeStep::GateBindHome_CanAskBind)
	{
		//请求资源分配
		Svr_GateBindHome_req *req = new Svr_GateBindHome_req();
		req->set_gateiid(EurekaClusterClient::instance().get_myiid());
		req->set_gatetoken(EurekaClusterClient::instance().get_token());

		EurekaClusterClient::instance().send_mth_protocol(req);

		bind_home_step = GateBindHomeStep::GateBindHome_AskBind;

		logDebug(out_net, "send gate bind home request.........");
	}

	//connect to router
	router_link_mth_.connect_to();
}

void GateServiceApp::on_homeservice_regist_result( HomeServiceLinkTo* plink)
{
	if (bind_home_step == GateBindHomeStep::GateBindHome_Confirm)
	{
		bind_home_step = GateBindHomeStep::GateBindHome_Done;
		plink->bind_home_confirm();
	}
}

void GateServiceApp::on_connection_timeout(GateSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_boot, "GateService listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void GateServiceApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void GateServiceApp::on_disconnected_with_homeservice(HomeServiceLinkTo* plink)
{
	bind_home_step = GateBindHomeStep::GateBindHome_CanAskBind;
}

void GateServiceApp::on_routerservice_regist_result( RouterServiceLinkTo* plink)
{
	router_link_mth_.on_linkto_regist_result( plink);
}

void GateServiceApp::on_disconnected_with_routerservice(RouterServiceLinkTo* plink)
{
	router_link_mth_.on_linkto_disconnected(plink);
}

void GateServiceApp::on_disconnected_with_gameservice(GameServiceLinkTo* plink)
{

}

void GateServiceApp::on_disconnected_with_unionservice(UnionClusterLinkTo* plink)
{

}