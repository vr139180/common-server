#include "GameServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>

USE_PROTOCOL_NAMESPACE

GameServiceApp& GameServiceApp::getInstance()
{
	static GameServiceApp s_instance_;
	return s_instance_;
}

GameServiceApp::GameServiceApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,is_ready_(false)
{
}

GameServiceApp::~GameServiceApp()
{
}

bool GameServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_GAME))
	{
		logFatal(out_runtime, "GameService load svr config file failed");
		return false;
	}

	GameConfig* cf = load_gameconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "GameService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

GameConfig* GameServiceApp::load_gameconfig()
{
	GameConfig* config = new GameConfig();
	std::unique_ptr<GameConfig> xptr(config);

	std::string fstr = "/system/game_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);

	return xptr.release();
}

bool GameServiceApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_HOME);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_GAME,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		gopt.eip.c_str(), gopt.eport, subscribe_types);

	return true;
}

bool GameServiceApp::init_network()
{
	int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator(500);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_runtime, ("GameService init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("GameService init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool GameServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	if (acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
	{
		logInfo(out_runtime, ("GameService listen socket at %s:%d \n"), cf.get_ip().c_str(), cf.get_port());
	}
	else
	{
		logFatal(out_runtime, ("GameService listen socket at %s:%d failed\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
	}

	char app_title_[200];
	sprintf(app_title_, "GameService VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

	OSSystem::mOS->SetAppTitle(app_title_);

	logDebug(out_gamedata, "xxxxxxx");

	return true;
}

void GameServiceApp::uninit_network()
{
	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	session_from_.unint_sessions();

	EurekaClusterClient::instance().uninit();
}

void GameServiceApp::uninit()
{
	acceptor_.reset();
}

void GameServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &GameServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&GameServiceApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void GameServiceApp::main_loop()
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

NetAcceptorEvent::NetSessionPtr GameServiceApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void GameServiceApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	GameSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_runtime, "me(GameService) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_runtime, "me(GameService) listen a connected request, and create a connection successfully");
	}
}

void GameServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
}

void GameServiceApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void GameServiceApp::on_connection_timeout(GameSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "GameService listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void GameServiceApp::on_disconnected_with_homeservice(HomeServiceLinkTo* plink)
{

}

void GameServiceApp::on_homeservice_regist_result(HomeServiceLinkTo* plink)
{

}

void GameServiceApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
{
	
}
