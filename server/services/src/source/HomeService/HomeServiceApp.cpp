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

#include "HomeServiceApp.h"

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

#include <taskLib/TaskMetaHome.h>

#include "dbs/DBSCtrl.h"

USE_PROTOCOL_NAMESPACE

HomeServiceApp& HomeServiceApp::getInstance()
{
	static HomeServiceApp s_instance_;
	return s_instance_;
}

HomeServiceApp::HomeServiceApp(): ServerAppBase()
,conf_(0)
,is_ready_(false)
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

HomeConfig* HomeServiceApp::get_config()
{
	return conf_.get();
}

bool HomeServiceApp::pre_init()
{
	lobby_hash_.init_vnode(800);
	this->all_lobbys_.reset(new LobbyService[HOME_LOBBY_THREADNUM]);
	for (int ii = 0; ii < HOME_LOBBY_THREADNUM; ++ii)
	{
		all_lobbys_[ii].init_lobby(ii);
		lobby_hash_.add_realnode( ii, ii);
	}
	lobby_hash_.build_nethashing();

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER);
	std::list< NETSERVICE_TYPE> router_types;

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_HOME,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		gopt.eip.c_str(), gopt.eport, subscribe_types, router_types, false);

	return true;
}

bool HomeServiceApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("HomeService init network failed"));
		return false;
	}

	return true;
}

bool HomeServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	dbsStore->init_dbsctrl();

	for (int ii = 0; ii < HOME_LOBBY_THREADNUM; ++ii)
	{
		all_lobbys_[ii].init(100);
		all_lobbys_[ii].start();
	}

	std::string verfmt = ShareUtil::str_format<128>(
		"HomeService VER:%s SVN:%s PID:%d Listen On PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

	OSSystem::mOS->SetAppTitle(verfmt.c_str());

	return true;
}

void HomeServiceApp::uninit_network()
{
	dbsStore->uninit_dbsctrl();

	NetDriverX::getInstance().uninitNetDriver();

	for (int ii = 0; ii < HOME_LOBBY_THREADNUM; ++ii)
		all_lobbys_[ii].stop();

	datarouter_link_mth_.free_all();

	EurekaClusterClient::instance().uninit();
}

void HomeServiceApp::uninit()
{
}

void HomeServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*5, boost::BOOST_BIND( &HomeServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
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

void HomeServiceApp::send_protocol_to_gate(BasicProtocol* pro)
{
	datarouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_GATE, pro);
}

void HomeServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	datarouter_link_mth_.connect_to();
}

void HomeServiceApp::on_datarouter_regist_result(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_regist_result(plink);
}

void HomeServiceApp::on_disconnected_with_datarouter(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_disconnected(plink);
}

void HomeServiceApp::dispatch_to_lobby(NetProtocol* msg)
{
	//根据用户userid来派发，而不是roleid
	S_INT_64 uid = msg->get_useriid();
	S_INT_32 lobbyind = lobby_hash_.get_netnode_byval(uid);
	LobbyService* plobby = &(all_lobbys_[lobbyind]);

	NETCMD_FUN_MAP fun = boost::bind(&LobbyService::NetProcessMessage, plobby,
		boost::placeholders::_1, boost::placeholders::_2);
	NetCommand *pcmd = new NetCommand( msg, fun);
	plobby->regist_netcmd(pcmd);
}
