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

#include "sEurekaApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/ShareUtil.h>

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
{
}

sEurekaApp::~sEurekaApp()
{
}

bool sEurekaApp::load_config()
{
	if( !ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_EUREKA))
    {
		logFatal(out_runtime, "Eureka load svr config file failed");
        return false;
    }

	EurekaConfig* cf = ConfigHelper::instance().load_eureka_config();
    if( cf == 0)
    {
		logFatal(out_runtime, "Eureka load config file failed");
        return false;
    }
	this->conf_.reset(cf);

	return true;
}

bool sEurekaApp::pre_init()
{
	session_from_.init_sessions(ConfigHelper::instance().get_globaloption().svrnum_min);

	eureka_ctrl_.init_ctrl();
	service_ctrl_.init_ctrl();

	return true;
}

bool sEurekaApp::init_network()
{
    int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal(out_runtime, ("sEureka init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal(out_runtime, ("sEureka init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	//初始化redis设置
	redis_.init_redis(conf_->redis_.ip_, conf_->redis_.port_, conf_->redis_.auth_,
		conf_->redis_.db_, conf_->redis_.socket_timeout_);
	redis_inthread_.reset(&redis_);

	return true;
}

bool sEurekaApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

    if( acceptor_->begin_listen( cf.get_ip().c_str(), cf.get_port(), cf.get_globaloption().svrnum_min))
    {
		logInfo(out_runtime, ("<<<<<<<<<<<<sEureka listen at %s:%d>>>>>>>>>>>> \n"), cf.get_ip().c_str(), cf.get_port());
    }
    else
    {
		logFatal(out_runtime, ("<<<<<<<<<<<<sEureka listen at %s:%d failed>>>>>>>>>>>>\n"), cf.get_ip().c_str(), cf.get_port());
		return false;
    }

	std::string verfmt = ShareUtil::str_format<128>(
		"sEureka VER:%s SVN:%s PID:%d Listen On PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

	OSSystem::mOS->SetAppTitle(verfmt.c_str());

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

	redis_inthread_.release();

	session_from_.unint_sessions();
}

void sEurekaApp::uninit()
{
	acceptor_.reset();
}

void sEurekaApp::register_timer()
{
	//regist timer for system
	//启动定时器
	this->add_apptimer(1000 * 1, boost::BOOST_BIND(&sEurekaApp::init_eureka_timer, this,
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
		CommandBase *pCmd = pop_one_cmd();
		std::unique_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loopnum;
			continue;
		}
		pCmd->run();
	}
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
		logError(out_runtime, "me(sEureka) listen a connected request, but refused by system");
		session_from_.free_from_wait_mth(pointer);
	}
	else
	{
		session_from_.ask_free_netsession_mth_confirm(pointer);
#ifdef EUREKA_DEBUGINFO_ENABLE
		logInfo(out_runtime, "me(sEureka) listen a connected request, and create a connection successfully");
#endif
	}
}

void sEurekaApp::on_connection_timeout(EurekaSession* session)
{
	ThreadLockWrapper guard(lock_);

	session->reset();

	session_from_.free_from_wait_mth(session);

	logError(out_runtime, "<<<<<< connection auth timeout, system cut connection by self");
}

void sEurekaApp::init_eureka_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	//定时器只在启动时执行一次
	finish = true;

	//初始化eureka控制
	eureka_ctrl_.boot_ctrl();
	eureka_ctrl_.init_timer();

	service_ctrl_.init_timer();
}

void sEurekaApp::service_maintnce_check(u64 tnow, int interval, u64 iid, bool& finish)
{
	ThreadLockWrapper guard(lock_);

	//登陆超时检测，和心跳激活
	session_from_.sessions_maintnce(tnow);
}

void sEurekaApp::on_disconnected_with_linkfrom(EurekaLinkFrom* plink)
{
	EurekaSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(lock_);

		eureka_ctrl_.on_disconnected_with_linkfrom(plink);

		session_from_.return_freesession_mth(psession);

		plink->reset();
		psession->reset();
	}
}