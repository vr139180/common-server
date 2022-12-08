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

#include "lobby/LobbyService.h"

#include <gameLib/commons/ConsistentHashChecker.h>
#include <gameLib/LogExt.h>

#include "config/HomeConfig.h"
#include "HomeServiceApp.h"

LobbyService::LobbyService():base()
{
}

LobbyService::~LobbyService()
{
	uninit_luacontext();
}

void LobbyService::thread_worker()
{
	//bind to thread
	svrApp.get_redisclient_thread().reset(&redis_);
	svrApp.get_rpcache_thread().reset(&redisproto_cache_);
	svrApp.get_luacontext_thread().reset(&lua_context_);

	int sleepstep = 0;
	while (1)
	{
		if (sleepstep >= loop_num_)
		{
			OSSystem::mOS->thread_yield();
			sleepstep = 0;
		}
		++sleepstep;

		if (will_quit_) break;

		CommandBase *pCmd = pop_one_cmd();
		std::unique_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loop_num_;
			continue;
		}

		pCmd->run();
	}

	svrApp.get_redisclient_thread().release();
	svrApp.get_rpcache_thread().release();
	svrApp.get_luacontext_thread().release();
}

void LobbyService::init_lobby()
{
	this->InitNetMessage();
	lobby_users_.init_cap(this, 2000);

	//初始化redis设置
	HomeConfig *conf_ = svrApp.get_config();
	redis_.init_redis(conf_->redis_.ip_, conf_->redis_.port_, conf_->redis_.auth_,
		conf_->redis_.db_, conf_->redis_.socket_timeout_);

	init_luacontext();
}

void LobbyService::vnode_cluster_maintance()
{
	SystemCommand<void>* cmd =
		new SystemCommand<void>(boost::bind(&LobbyService::on_vnode_cluster_maintance, this, this));
	this->regist_syscmd(cmd);
}

LobbyUser* LobbyService::get_usercheck_from_msg(NetProtocol* msg)
{
	LobbyUser* puser = lobby_users_.get_lobbyuser(msg->get_useriid());
	if (puser->is_samesession(msg->head_))
		return puser;
	return 0;
}

LobbyUser* LobbyService::get_userbyid_from_msg(NetProtocol* msg)
{
	return lobby_users_.get_lobbyuser(msg->get_useriid());
}

LobbyUser* LobbyService::get_userbyid_from_msg(const SProtocolHead& head)
{
	return lobby_users_.get_lobbyuser( head.get_token_useriid());
}

void LobbyService::on_vnode_cluster_maintance(void*)
{
	std::list<LobbyUser*> allusers;
	lobby_users_.get_all_users(allusers);

	//根据新的hash规则，去除不属于本节点管理的user数据
	for (std::list<LobbyUser*>::iterator iter = allusers.begin(); iter != allusers.end(); ++iter)
	{
		LobbyUser* puser = (*iter);
		if (!ConsistentHashChecker::instance().is_need_recircle_cluster(puser->get_user_iid()))
			continue;
		puser->save_all(false);

		logDebug(out_runtime, "home maintance, user:%lld be removed from this home cluster node", puser->get_user_iid());
		lobby_users_.free_lobbyuser(puser->get_user_iid());
	}
}
