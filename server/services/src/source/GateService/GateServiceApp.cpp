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

#include "GateServiceApp.h"

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
{
}

GateServiceApp::~GateServiceApp()
{
}

bool GateServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_GATE))
	{
		logFatal(out_runtime, "GateService load svr config file failed");
		return false;
	}

	GateConfig* cf = load_gateconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "GateService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

GateConfig* GateServiceApp::load_gateconfig()
{
	GateConfig* config = new GateConfig();
	std::unique_ptr<GateConfig> xptr(config);

	std::string fstr = "/system/gate_config.xml";
	std::string str = ConfigTool::get_instance().get_txtfilecontent(fstr.c_str(), true);

	tinyxml2::XMLDocument doc;
	if (!XmlUtil::loadfromstring(str.c_str(), doc))
		return 0;

	tinyxml2::XMLElement* root = doc.RootElement();

	config->loopnum_ = XmlUtil::GetXmlAttrInt(root, "loopnum", 100);
	std::string ws = ConfigHelper::instance().find_ext(GATE_INSTANCE_EXTPARAM);
	ShareUtil::to_lower(ws);
	config->set_gate_type(ws);

	return xptr.release();
}

bool GateServiceApp::pre_init()
{
	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER);
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER);
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER);
	std::list<NETSERVICE_TYPE> router_types;

	EurekaNodeInfo enode;
	if (!EurekaClusterClient::get_eureka_masterinfo(gopt.eureka_.c_str(), enode))
	{
		logError(out_runtime, "xxxxxxxxxx-- access url:%s get eureka master failed......", gopt.eureka_.c_str());
		return false;
	}

	EurekaServerExtParam exts;
	exts[GATE_INSTANCE_EXTPARAM] = std::to_string((S_INT_32)conf_->gate_type_);
	exts[GATE_WIP] = ConfigHelper::instance().find_ext(GATE_WIP);
	exts[GATE_WPORT] = ConfigHelper::instance().find_ext(GATE_WPORT);

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_GATE,
		cf.get_ip().c_str(), cf.get_port(), exts, enode, subscribe_types, router_types);

	GamePlayerCtrl::instance().init_gameplayerctrl( GATE_PLAYER_MAX);

	return true;
}

bool GateServiceApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("GateService init network failed"));
		return false;
	}

	if( acceptor_.get() != 0)
	{
		logFatal( out_runtime, ("GateService init network failed"));
		return false;
	}

	acceptor_.reset( new NetAcceptor( *this));

	return true;
}

bool GateServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	GamePlayerCtrl::instance().start();

	std::string verfmt = ShareUtil::str_format<128>(
		"GateService VER:%s SVN:%s PID:%d Listen On PORT: %d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId(), cf.get_port());

    OSSystem::mOS->SetAppTitle( verfmt.c_str());

	return true;
}

void GateServiceApp::uninit_network()
{
	GamePlayerCtrl::instance().stop();

	if (acceptor_.get())
		acceptor_->end_listen();
	NetDriverX::getInstance().uninitNetDriver();

	datarouter_link_mth_.free_all();
	svrrouter_link_mth_.free_all();
	fightrouter_link_mth_.free_all();

	EurekaClusterClient::instance().uninit();
	GamePlayerCtrl::instance().uninit_gameplayerctrl();
}

void GateServiceApp::uninit()
{
	acceptor_.reset();
}

void GateServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*15, boost::BOOST_BIND( &GateServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	//5秒维护一次 登陆超时
	this->add_apptimer(1000 * 5, boost::BOOST_BIND(&GamePlayerCtrl::maintnce_proxylogin_timer, &GamePlayerCtrl::instance(),
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void GateServiceApp::main_loop()
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

NetAcceptorEvent::NetSessionPtr GateServiceApp::ask_free_netsession()
{
	GamePlayer* player = GamePlayerCtrl::instance().ask_free_slot( get_gateid());
	if (player == 0)
	{
		NetAcceptorEvent::NetSessionPtr new_session;
		return new_session;
	}

	return player->get_session();
}

void GateServiceApp::accept_netsession( NetAcceptorEvent::NetSessionPtr session, bool refuse, int err)
{
	GamePlayer* player = dynamic_cast<GamePlayer*>(session->get_bindevent());
	if (player == 0)
		return;

	if (refuse)
		player->reuse();

	//remove from waiting list
	if (refuse)
	{
		logError(out_runtime, "me(GateService) listen a connected request, but refused by system");

		GamePlayerCtrl::instance().return_slot_to_free(player->get_userslot());
	}
	else
	{
		logInfo(out_runtime, "me(GateService) listen a connected request, and create a connection successfully");
	}
}

S_INT_64 GateServiceApp::get_gateid()
{
	return EurekaClusterClient::instance().get_myiid();
}

void GateServiceApp::route_to_datarouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro)
{
	logDebug(out_runtime, "recv msg:%d from player, router to datarouter", pro->get_msg());
	datarouter_link_mth_.send_mth_protocol(to, pro);
}

void GateServiceApp::send_to_datarouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg)
{
	datarouter_link_mth_.send_mth_protocol(to, msg);
}

void GateServiceApp::route_to_servicerouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro)
{
	svrrouter_link_mth_.send_mth_protocol(to, pro);
}

void GateServiceApp::send_to_servicerouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg)
{
	svrrouter_link_mth_.send_mth_protocol(to, msg);
}

void GateServiceApp::route_to_fightrouter(PRO::ERK_SERVICETYPE to, NetProtocol* pro)
{
	fightrouter_link_mth_.send_mth_protocol(to, pro);
}

void GateServiceApp::send_to_fightrouter(PRO::ERK_SERVICETYPE to, BasicProtocol* msg)
{
	fightrouter_link_mth_.send_mth_protocol(to, msg);
}

void GateServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	//connect to router
	datarouter_link_mth_.connect_to();
	svrrouter_link_mth_.connect_to();
	fightrouter_link_mth_.connect_to();
}

void GateServiceApp::on_datarouter_regist_result( DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_regist_result( plink);
}

void GateServiceApp::on_disconnected_with_datarouter(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_disconnected(plink);
}

void GateServiceApp::on_svrrouter_regist_result(ServiceRouterLinkTo* plink)
{
	svrrouter_link_mth_.on_linkto_regist_result(plink);
}

void GateServiceApp::on_disconnected_with_svrrouter(ServiceRouterLinkTo* plink)
{
	svrrouter_link_mth_.on_linkto_disconnected(plink);
}

void GateServiceApp::on_disconnected_with_fightrouter(FightRouterLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_disconnected(plink);
}

void GateServiceApp::on_fightrouter_regist_result(FightRouterLinkTo* plink)
{
	fightrouter_link_mth_.on_linkto_regist_result(plink);
}

void GateServiceApp::dispatch_gate_process(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	PlayerChannel* pchannel = GamePlayerCtrl::instance().get_channel_by_head(pro->head_);
	if (pchannel)
	{
		NETCMD_FUN_MAP fun = boost::bind(&PlayerChannel::NetProcessMessage, pchannel,
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		pchannel->regist_netcmd(pcmd);
	}
}
