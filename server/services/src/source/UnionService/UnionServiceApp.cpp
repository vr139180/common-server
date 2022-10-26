#include "UnionServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>

USE_PROTOCOL_NAMESPACE

UnionServiceApp& UnionServiceApp::getInstance()
{
	static UnionServiceApp s_instance_;
	return s_instance_;
}

UnionServiceApp::UnionServiceApp(): ServerAppBase()
,conf_( 0)
,is_ready_(false)
{
}

UnionServiceApp::~UnionServiceApp()
{
}

bool UnionServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_UNION))
	{
		logFatal(out_runtime, "UnionService load svr config file failed");
		return false;
	}

	UnionConfig* cf = load_unionconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "UnionService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

UnionConfig* UnionServiceApp::load_unionconfig()
{
	UnionConfig* config = new UnionConfig();
	std::unique_ptr<UnionConfig> xptr(config);

	std::string fstr = "/system/union_config.xml";
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

bool UnionServiceApp::pre_init()
{
	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_UNION,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		gopt.eip.c_str(), gopt.eport, subscribe_types);

	return true;
}

bool UnionServiceApp::init_network()
{
    int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator(500);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_runtime, ("UnionService init network failed"));
		return false;
	}

	return true;
}

bool UnionServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

    char app_title_[200];
    sprintf(app_title_, "UnionService VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void UnionServiceApp::uninit_network()
{
	NetDriverX::getInstance().uninitNetDriver();

	EurekaClusterClient::instance().uninit();
}

void UnionServiceApp::uninit()
{
}

void UnionServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &UnionServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void UnionServiceApp::main_loop()
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

void UnionServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
}
