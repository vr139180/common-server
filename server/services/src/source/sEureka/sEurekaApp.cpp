#include "sEurekaApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>

#include <gameLib/LogExt.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>

USE_PROTOCOL_NAMESPACE

sEurekaApp& sEurekaApp::getInstance()
{
	static sEurekaApp s_instance_;
	return s_instance_;
}

sEurekaApp::sEurekaApp(): ServerAppBase()
,acceptor_( 0)
,conf_( 0)
,service_threads_num_( 0)
{
}

sEurekaApp::~sEurekaApp()
{
}

bool sEurekaApp::load_config()
{
	if( !ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_EUREKA))
    {
		logFatal( out_boot, "Eureka load svr config file failed");
        return false;
    }

	EurekaConfig* cf = ConfigHelper::instance().load_eureka_config();
    if( cf == 0)
    {
		logFatal( out_boot, "Eureka load config file failed");
        return false;
    }
	this->conf_.reset(cf);

	load_redisscript();

	return true;
}

#include "rediscript/homegate_script.h"
void sEurekaApp::load_redisscript()
{
	std::string fstr;
	fstr = "/system/eureka/gatebindhome_request.lua";
	HomeGateConst::gatebindhome_request = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	fstr = "/system/eureka/gatebindhome_confirm.lua";
	HomeGateConst::gatebindhome_confirm = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	fstr = "/system/eureka/gatebindhome_maintnce.lua";
	HomeGateConst::gatebindhome_maintnce = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	fstr = "/system/eureka/gatebindhome_authtimeout.lua";
	HomeGateConst::gatebindhome_authtimeout = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

}

bool sEurekaApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	service_ctrl_.init_ctrl();

	return true;
}

bool sEurekaApp::init_network()
{
    int cpu = ConfigHelper::instance().get_cpunum();
	//MutexAllocator::getInstance().init_allocator( cpu*10);

	cpu =cpu*2+2;
	if( !NetDriverX::getInstance().initNetDriver(cpu))
	{
		logFatal( out_boot, ("sEureka init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_boot, ("sEureka init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	//初始化redis设置
	redis_.init_redis(conf_->redis_.ip_, conf_->redis_.port_, conf_->redis_.auth_,
		conf_->redis_.db_, conf_->redis_.socket_timeout_);
	redis_inthread_.reset(&redis_);

	//init service threads
	service_threads_num_ = conf_->service_thread_num_;
	services_.reset(new EurekaService[service_threads_num_]);
	for (int ii = 0; ii < service_threads_num_; ++ii)
	{
		services_[ii].init( conf_.get());
		services_[ii].start();
	}

	return true;
}

bool sEurekaApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

    if( acceptor_->begin_listen( cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
    {
		logInfo( out_boot, ("<<<<<<<<<<<<sEureka listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
    }
    else
    {
		logFatal( out_boot, ("<<<<<<<<<<<<sEureka listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
    }

    char app_title_[200];
    sprintf(app_title_, "sEureka VER: %s REV: %s PID: %d PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( app_title_ );

	return true;
}

void sEurekaApp::uninit_network()
{
	if (acceptor_.get())
		acceptor_->end_listen();

	NetDriverX::getInstance().uninitNetDriver();

	service_ctrl_.uninit_ctrl();
	//从redis注销
	eureka_ctrl_.unint_ctrl();

	for (int ii = 0; ii < service_threads_num_; ++ii)
		services_[ii].stop();
	services_.reset(0);

	redis_inthread_.release();

	session_from_.unint_sessions();
}

void sEurekaApp::uninit()
{
	acceptor_.reset(0);
}

void sEurekaApp::register_timer()
{
	//regist timer for system
	this->add_apptimer(1000 * 5, boost::BOOST_BIND(&sEurekaApp::init_eureka_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	this->add_apptimer(1000 * 15, boost::BOOST_BIND(&sEurekaApp::service_maintnce_check, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void sEurekaApp::main_loop()
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

		//主线程只处理syscommand命令
		CommandBase *pCmd = svrApp.pop_sys_cmd();
		std::auto_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loopnum;
			continue;
		}
		pCmd->run();
	}
}

CommandBase* sEurekaApp::pop_net_cmd()
{
	if (will_quit_app_) return 0;

#define ANOTHER_CMD_LIST( cur)	(cur==1?0:1)

	CommandBase* pCmd = 0;

	ThreadLockWrapper guard(lock_);

	if (net_cmds_[cur_netcmds_].size() > 0)
	{
		pCmd = net_cmds_[cur_netcmds_].front();
		net_cmds_[cur_netcmds_].pop_front();
	}
	else if (user_cmds_[cur_usercmds_].size() > 0)
	{
		pCmd = user_cmds_[cur_usercmds_].front();
		user_cmds_[cur_usercmds_].pop_front();
	}

	if (net_cmds_[cur_netcmds_].size() == 0)
	{
		if (net_cmds_[ANOTHER_CMD_LIST(cur_netcmds_)].size() > 0)
		{
			cur_netcmds_ = ANOTHER_CMD_LIST(cur_netcmds_);
		}
		else if (user_cmds_[cur_usercmds_].size() == 0)
		{
			if (user_cmds_[ANOTHER_CMD_LIST(cur_usercmds_)].size() > 0)
			{
				cur_usercmds_ = ANOTHER_CMD_LIST(cur_usercmds_);
			}
		}
	}

	return pCmd;
}

void sEurekaApp::remove_waitsession_no_mutex(EurekaSession* psession)
{
	session_from_.remove_waitsession_mth(psession);
}

void sEurekaApp::return_freesession_no_mutext(EurekaSession* psession)
{
	session_from_.return_freesession_mth(psession);
}

NetAcceptorEvent::NetSessionPtr sEurekaApp::ask_free_netsession()
{
	ThreadLockWrapper guard(lock_);

	return session_from_.ask_free_netsession_mth();
}

void sEurekaApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	EurekaSession *pointer = session_from_.get_sessionlink_by_session(session);
	if (pointer == 0)
		return;

	if (refuse)
		pointer->reset();

	ThreadLockWrapper guard(lock_);

	//remove from waiting list
	if (refuse)
	{
		logError(out_boot, "me(sEureka) listen a connected request, but refused by system");
		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
#ifdef EUREKA_DEBUGINFO_ENABLE
		logInfo(out_net, "me(sEureka) listen a connected request, and create a connection successfully");
#endif
	}
}

void sEurekaApp::on_connection_timeout(EurekaSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_net, "<<<<<< connection auth timeout, system cut connection by self");
}

void sEurekaApp::init_eureka_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	//定时器只在启动时执行一次
	finish = true;

	//初始化eureka控制
	eureka_ctrl_.init_ctrl();
	eureka_ctrl_.init_timer();

	service_ctrl_.init_timer();

	eureka_ctrl_.mark_boosted();
}

void sEurekaApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	//登陆超时检测，和心跳激活
	session_from_.sessions_maintnce(tnow);
}