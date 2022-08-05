#include "HomeServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <gameLib/global_const.h>

#include <taskLib/TaskMetaHome.h>

#include "dbs/DBSCtrl.h"

USE_PROTOCOL_NAMESPACE

HomeServiceApp& HomeServiceApp::getInstance()
{
	static HomeServiceApp s_instance_;
	return s_instance_;
}

HomeServiceApp::HomeServiceApp(): ServerAppBase()
,acceptor_( 0)
,conf_(0)
,is_ready_(false)
,lobby_nums_(0)
{
}

HomeServiceApp::~HomeServiceApp()
{
}

bool HomeServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_HOME))
	{
		logFatal(out_runtime, "HomeService load svr config file failed");
		return false;
	}

	HomeConfig* cf = load_homeconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "HomeService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	if (!TaskMetaHome::instance().load_taskmetas())
	{
		logFatal(out_runtime, "HomeService load task system config file failed");
		return false;
	}

	return true;
}

HomeConfig* HomeServiceApp::load_homeconfig()
{
	HomeConfig* config = new HomeConfig();
	std::unique_ptr<HomeConfig> xptr(config);

	std::string fstr = "/system/home_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	config->service_thread_num_ = XmlUtil::GetXmlAttrInt(root, "service_thread_num", 4);
	config->db_thread_nums_ = XmlUtil::GetXmlAttrInt(root, "dbs", 2);

	tinyxml2::XMLElement* rds = root->FirstChildElement("redis");
	if (rds == 0)
		return 0;

	config->redis_.load_from_xml(rds);

	tinyxml2::XMLElement* db = root->FirstChildElement("db");
	if (db == 0)
		return 0;

	config->db_.load_from_xml(db);

	return xptr.release();
}

bool HomeServiceApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	all_lobby_users_.init_separate(GATEHOME_GROUP_NUM, GATEHOME_PIECE_NUM);
	this->lobby_nums_ = all_lobby_users_.get_max_piece();
	this->all_lobbys_.reset(new LobbyService[lobby_nums_]);
	for (int ii = 0; ii < lobby_nums_; ++ii)
	{
		std::vector<LobbyUser*> us;
		all_lobby_users_.get_piece_data(ii, us);
		all_lobbys_[ii].init_lobby(ii, all_lobby_users_.get_piece_slotnum(), us);
	}

	gate_link_map_.init_holder();

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_RES);
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_HOME,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		gopt.eip.c_str(), gopt.eport, subscribe_types);

	return true;
}

bool HomeServiceApp::init_network()
{
	int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator(500);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_runtime, ("HomeService init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("HomeService init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool HomeServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

    if( acceptor_->begin_listen(cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
    {
		logInfo(out_runtime, ("<<<<<<<<<<<<HomeService listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
    }
    else
    {
		logFatal(out_runtime, ("<<<<<<<<<<<<HomeService listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
    }

	dbsStore->init_dbsctrl();

	for (int ii = 0; ii < lobby_nums_; ++ii)
	{
		all_lobbys_[ii].init(100);
		all_lobbys_[ii].start();
	}

    char app_title_[200];
    sprintf(app_title_, "HomeService VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void HomeServiceApp::uninit_network()
{
	dbsStore->uninit_dbsctrl();

	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	for (int ii = 0; ii < lobby_nums_; ++ii)
		all_lobbys_[ii].stop();

	gate_link_map_.uninit_holder();
	session_from_.unint_sessions();
	fightrouter_link_mth_.free_all();

	EurekaClusterClient::instance().uninit();
}

void HomeServiceApp::uninit()
{
	acceptor_.reset();
}

void HomeServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*5, boost::BOOST_BIND( &HomeServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&HomeServiceApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

LobbyService* HomeServiceApp::get_lobbysvr_by_slot(int slot)
{
	int ind = all_lobby_users_.get_pieceindex_from_slot(slot);
	if (ind == -1)
		return 0;
	return &(all_lobbys_[ind]);
}

void HomeServiceApp::main_loop()
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

NetAcceptorEvent::NetSessionPtr HomeServiceApp::ask_free_netsession()
{
	if (!is_ready_)
	{
		NetAcceptorEvent::NetSessionPtr new_session;
		return new_session;
	}

	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void HomeServiceApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	HomeSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_runtime, "me(HomeService) listen a connected request, but refused by system");

		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
		logInfo(out_runtime, "me(HomeService) listen a connected request, and create a connection successfully");
	}
}

void HomeServiceApp::send_protocol_to_fightrouter(BasicProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(pro);
}

void HomeServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	//connect ro res service
	res_link_mth_.connect_to();
	fightrouter_link_mth_.connect_to();
}

void HomeServiceApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	session_from_.sessions_maintnce(tnow);
}

void HomeServiceApp::on_connection_timeout(HomeSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "HomeService listen a connected request, but this connection don't finish auth in a request time. system cut connection by self");
}

void HomeServiceApp::on_mth_servicebindservice_req(BasicProtocol* pro, bool& autorelease, void* session)
{
	Svr_ServiceBindService_req* req = dynamic_cast<Svr_ServiceBindService_req*>(pro);
	HomeSession* psession = reinterpret_cast<HomeSession*>(session);

	Svr_ServiceBindService_ack* ack = new Svr_ServiceBindService_ack();
	ack->set_result(1);
	ack->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_HOME);
	ack->set_toiid(req->toiid());
	ack->set_totoken(req->totoken());

	//service没有变化
	if (req->toiid() != EurekaClusterClient::instance().get_myiid()
		|| req->totoken() != EurekaClusterClient::instance().get_token())
	{
		ack->set_result(1);
		psession->send_protocol(ack);
		return;
	}

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
	{
		ack->set_result(1);
		psession->send_protocol(ack);
		return;
	}

	//来自gate的注册
	if (ctype == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		ThreadLockWrapper guard(get_threadlock());

		session_from_.remove_waitsession_mth(psession);
		GateServiceLinkFrom *pLink = gate_link_map_.ask_free_link();

		pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//设置当前gatelinke
		gate_link_ = gate_link_map_.regist_onlinelink(pLink);

		pLink->registinfo_tolog(true);

		ack->set_result(0);
	}
	else // other sevices
	{

	}

	psession->send_protocol(ack);
}

void HomeServiceApp::on_disconnected_with_gateservice(GateServiceLinkFrom* plink)
{
	HomeSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(get_threadlock());

		//断开映射关系
		gate_link_map_.return_freelink(plink);
		if (plink == gate_link_)
			gate_link_ = 0;

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}

void HomeServiceApp::send_protocol_to_res(BasicProtocol* pro)
{
	res_link_mth_.send_mth_protocol(pro);
}

void HomeServiceApp::send_protocol_to_gate(BasicProtocol* pro)
{
	if (gate_link_ != 0)
		gate_link_->send_protocol(pro);
	else
	{
		delete pro;
	}
}

void HomeServiceApp::on_resservice_regist_result( ResClusterLinkTo* plink)
{
	res_link_mth_.on_linkto_regist_result( plink);
}

void HomeServiceApp::on_disconnected_with_resservice(ResClusterLinkTo* plink)
{
	res_link_mth_.on_linkto_disconnected(plink);
}

void HomeServiceApp::on_disconnected_with_unionservice(UnionClusterLinkTo* plink)
{

}

void HomeServiceApp::post_syscmd_2_lobbyservice(S_INT_64 token, CommandBase* pcmd)
{
	int slot = 0;
	ProtoTokenUtil::parse_usertoken2(token, slot);
	LobbyService* pls = get_lobbysvr_by_slot(slot);
	if (pls == 0)
	{
		delete pcmd;
		return;
	}

	pls->regist_syscmd(pcmd);
}

void HomeServiceApp::on_fightrouterservice_regist_result(FightRouterServiceLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_regist_result(plink);
}

void HomeServiceApp::on_disconnected_with_fightrouterservice(FightRouterServiceLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_disconnected(plink);
}