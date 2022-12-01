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

#include "GameServiceApp.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/Version.h>
#include <cmsLib/util/XmlUtil.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/config/ConfigTool.h>
#include <worldsLib/GameRegionMeta.h>
#include <worldsLib/utils/WorldUtil.h>

USE_PROTOCOL_NAMESPACE

GameServiceApp& GameServiceApp::getInstance()
{
	static GameServiceApp s_instance_;
	return s_instance_;
}

GameServiceApp::GameServiceApp(): ServerAppBase()
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

	std::string strregion = ConfigHelper::instance().find_ext(GAME_EXT_PARAMS_KEY);
	if (strregion == "")
	{
		logFatal(out_runtime, "gameservice svn config, need to setup 'region' exts");
		return false;
	}

	S_INT_32 regionid = ShareUtil::atoi(strregion.c_str());
	if (!GameRegionMeta::instance().load_region_config(regionid))
	{
		logFatal(out_runtime, "gameservice load cmsworld/regions/region_%d.xml failed", regionid);
		return false;
	}

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
	config->channel_num_ = XmlUtil::GetXmlAttrInt(root, "channels", 1);

	return xptr.release();
}

GameConfig* GameServiceApp::get_config()
{
	return conf_.get();
}

bool GameServiceApp::pre_init()
{
	channel_num_ = get_config()->channel_num_;
	this->all_channels_.reset(new RegionChannelService[channel_num_]);
	for (int ii = 0; ii < channel_num_; ++ii)
	{
		all_channels_[ii].init_channel( ii);
	}

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER);
	std::list< NETSERVICE_TYPE> router_types;

	EurekaNodeInfo enode;
	if (!EurekaClusterClient::get_eureka_masterinfo(gopt.eureka_.c_str(), enode))
	{
		logError(out_runtime, "xxxxxxxxxx-- access url:%s get eureka master failed......", gopt.eureka_.c_str());
		return false;
	}

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_GAME,
		cf.get_ip().c_str(), cf.get_port(), cf.get_exts(),
		enode, subscribe_types, router_types, false);

	return true;
}

bool GameServiceApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("GameService init network failed"));
		return false;
	}

	return true;
}

bool GameServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	for (int ii = 0; ii < channel_num_; ++ii)
	{
		all_channels_[ii].init(100);
		all_channels_[ii].start();
	}

	char app_title_[200];
	sprintf(app_title_, "GameService VER: %s REV: %s PID: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId());

	OSSystem::mOS->SetAppTitle(app_title_);

	return true;
}

void GameServiceApp::uninit_network()
{
	NetDriverX::getInstance().uninitNetDriver();

	for (int ii = 0; ii < channel_num_; ++ii)
	{
		all_channels_[ii].stop();
	}

	fightrouter_link_mth_.free_all();

	EurekaClusterClient::instance().uninit();
}

void GameServiceApp::uninit()
{
}

void GameServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &GameServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void GameServiceApp::main_loop()
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

void GameServiceApp::send_protocol_to_gate(BasicProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_GATE, pro);
}

void GameServiceApp::send_protocol_to_gate(NetProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_GATE, pro);
}

void GameServiceApp::send_protocol_to_home(BasicProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_HOME, pro);
}

void GameServiceApp::send_protocol_to_home(NetProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(PRO::ERK_SERVICE_HOME, pro);
}

void GameServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	fightrouter_link_mth_.connect_to();
}

void GameServiceApp::on_disconnected_with_fightrouter(FightRouterLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_disconnected(plink);
}

void GameServiceApp::on_fightrouter_regist_result(FightRouterLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_regist_result(plink);
}

void GameServiceApp::dispatch_msg_to_channel(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	S_INT_64 gameid = pro->get_gameid();
	S_INT_32 channel = WorldUtil::get_channel_from_gameid(gameid);
	if (channel >= channel_num_ || channel < 0)
		return;

	RegionChannelService* psvr = &(all_channels_[channel]);

	NETCMD_FUN_MAP fun = boost::bind(&RegionChannelService::NetProcessMessage, psvr,
		boost::placeholders::_1, boost::placeholders::_2);
	NetCommand *pcmd = new NetCommand( p_msg.release(), fun);
	psvr->regist_netcmd(pcmd);
}

void GameServiceApp::assign_user_to_channel(NetProtocol* pro)
{
	//选出性能较优的channel
	S_INT_32 cid = 0;
	for (int ii = 0; ii < channel_num_; ++ii)
	{
		RegionChannelService* pservice = &(all_channels_[ii]);
		if (!pservice->is_max_performance())
		{
			cid = ii;
			break;
		}
	}

	RegionChannelService* psvr = &(all_channels_[cid]);

	NETCMD_FUN_MAP fun = boost::bind(&RegionChannelService::NetProcessMessage, psvr,
		boost::placeholders::_1, boost::placeholders::_2);
	NetCommand *pcmd = new NetCommand( pro, fun);
	psvr->regist_netcmd(pcmd);
}
