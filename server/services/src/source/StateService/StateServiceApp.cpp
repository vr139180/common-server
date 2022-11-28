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

#include "StateServiceApp.h"

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

#include "dbs/DBSCtrl.h"

USE_PROTOCOL_NAMESPACE

StateServiceApp& StateServiceApp::getInstance()
{
	static StateServiceApp s_instance_;
	return s_instance_;
}

StateServiceApp::StateServiceApp(): ServerAppBase()
, conf_( 0)
, is_ready_( false)
, cur_state_index_( 0)
{
}

StateServiceApp::~StateServiceApp()
{
}

bool StateServiceApp::load_config()
{
	if (!ConfigHelper::instance().init_config(NETSERVICE_TYPE::ERK_SERVICE_STATE))
	{
		logFatal(out_runtime, "StateService load svr config file failed");
		return false;
	}

	StateConfig* cf = load_stateconfig();
	if (cf == 0)
	{
		logFatal(out_runtime, "StateService load config file failed");
		return false;
	}

	this->conf_.reset(cf);

	return true;
}

StateConfig* StateServiceApp::load_stateconfig()
{
	StateConfig* config = new StateConfig();
	std::unique_ptr<StateConfig> xptr(config);

	std::string fstr = "/system/state_config.xml";
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

StateConfig* StateServiceApp::get_config()
{
	return conf_.get();
}

bool StateServiceApp::pre_init()
{
	this->all_states_.reset(new StateService[STATE_SERVICE_THREADNUM]);
	for (int ii = 0; ii < STATE_SERVICE_THREADNUM; ++ii)
	{
		all_states_[ii].init_state();
	}

	//eureka init
	ConfigHelper& cf = ConfigHelper::instance();
	const config::GlobalOption& gopt = cf.get_globaloption();

	std::list< NETSERVICE_TYPE> subscribe_types;
	subscribe_types.push_back(NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER);
	std::list< NETSERVICE_TYPE> router_types;

	EurekaNodeInfo enode;
	if (!EurekaClusterClient::get_eureka_masterinfo(gopt.eureka_.c_str(), enode))
	{
		logError(out_runtime, "xxxxxxxxxx-- access url:%s get eureka master failed......", gopt.eureka_.c_str());
		return false;
	}

	EurekaClusterClient::instance().init(this, NETSERVICE_TYPE::ERK_SERVICE_STATE,
		cf.get_ip().c_str(), cf.get_port(), EurekaServerExtParam(),
		enode, subscribe_types, router_types, false);

	return true;
}

bool StateServiceApp::init_network()
{
	int neths = ConfigHelper::instance().get_netthreads();
	if( !NetDriverX::getInstance().initNetDriver(neths))
	{
		logFatal( out_runtime, ("StateService init network failed"));
		return false;
	}

	return true;
}

bool StateServiceApp::init_finish()
{
	ConfigHelper& cf = ConfigHelper::instance();

	dbsStore->init_dbsctrl();

	for (int ii = 0; ii < STATE_SERVICE_THREADNUM; ++ii)
	{
		all_states_[ii].init(100);
		all_states_[ii].start();
	}

	std::string verfmt = ShareUtil::str_format<128>(
		"StateService VER:%s SVN:%s PID:%d\n",
		get_version().c_str(), get_svn_reversion().c_str(), OSSystem::mOS->GetProcessId());

	OSSystem::mOS->SetAppTitle(verfmt.c_str());

	return true;
}

void StateServiceApp::uninit_network()
{
	dbsStore->uninit_dbsctrl();

	NetDriverX::getInstance().uninitNetDriver();

	for (int ii = 0; ii < STATE_SERVICE_THREADNUM; ++ii)
		all_states_[ii].stop();

	datarouter_link_mth_.free_all();

	EurekaClusterClient::instance().uninit();
}

void StateServiceApp::uninit()
{
}

void StateServiceApp::register_timer()
{
	//regist timer for system
	//auto connect
	this->add_apptimer( 1000*5, boost::BOOST_BIND( &StateServiceApp::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	this->add_apptimer( 1000*2, boost::BOOST_BIND(&StateServiceApp::online_user_maintance_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	this->add_apptimer(1000 * 10, boost::BOOST_BIND(&StateServiceApp::offline_user_maintance_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	EurekaClusterClient::instance().regist_timer();
}

void StateServiceApp::main_loop()
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

StateService* StateServiceApp::get_next_dispatcher()
{
	++cur_state_index_;
	if (cur_state_index_ >= STATE_SERVICE_THREADNUM)
		cur_state_index_ = 0;

	return &(all_states_[cur_state_index_]);
}

void StateServiceApp::send_to_gate( SProtocolHead& head, BasicProtocol* pro)
{
	head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_STATE;
	head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

	datarouter_link_mth_.send_mth_protocol( head, pro);
}

void StateServiceApp::send_to_datarouter(NetProtocol* pro)
{
	SProtocolHead& head = pro->write_head();
	head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_STATE;
	head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER;

	datarouter_link_mth_.send_mth_protocol(pro);
}

void StateServiceApp::auto_connect_timer( u64 tnow, int interval, u64 iid, bool& finish)
{
	datarouter_link_mth_.connect_to();
}

void StateServiceApp::on_datarouter_regist_result(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_regist_result(plink);
}

void StateServiceApp::on_disconnected_with_datarouter(DataRouterLinkTo* plink)
{
	datarouter_link_mth_.on_linkto_disconnected(plink);
}

void StateServiceApp::online_user_maintance_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	//每次随机派发到一个线程处理
	StateService* pstate = get_next_dispatcher();
	SystemCommand<void>* pcmd = new SystemCommand<void>(boost::bind(&StateService::on_onlineuser_maintance, pstate, pstate));
	pstate->regist_syscmd(pcmd);
}

void StateServiceApp::offline_user_maintance_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	StateService* pstate = get_next_dispatcher();
	SystemCommand<void>* pcmd = new SystemCommand<void>(boost::bind(&StateService::on_offlineuser_maintance, pstate, pstate));
	pstate->regist_syscmd(pcmd);
}

void StateServiceApp::dispath_userlogout_process(S_INT_64 userid)
{
	StateService* pstate = get_next_dispatcher();
	SystemCommand2<S_INT_64>* pcmd = new SystemCommand2<S_INT_64>(
		boost::bind(&StateService::on_user_logout_process, pstate, boost::placeholders::_1), userid);
	pstate->regist_syscmd(pcmd);
}
