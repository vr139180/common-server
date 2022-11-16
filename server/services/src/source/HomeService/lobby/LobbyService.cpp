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

#include "config/HomeConfig.h"
#include "HomeServiceApp.h"

LobbyService::LobbyService():base(),piece_index_(-1)
{
}

LobbyService::~LobbyService()
{
	uninit_luacontext();
	users_.clear();
}

void LobbyService::thread_worker()
{
	OSSystem::mOS->UpdateNowTick();
	u64 st = OSSystem::mOS->GetTicks();

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

		//update time
		OSSystem::mOS->UpdateNowTick();

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

void LobbyService::init_lobby(int pindex)
{
	this->piece_index_ = pindex;

	this->InitNetMessage();

	//³õÊ¼»¯redisÉèÖÃ
	HomeConfig *conf_ = svrApp.get_config();
	redis_.init_redis(conf_->redis_.ip_, conf_->redis_.port_, conf_->redis_.auth_,
		conf_->redis_.db_, conf_->redis_.socket_timeout_);

	init_luacontext();
}

void LobbyService::reset_lobby( void*)
{

}

void LobbyService::reset_syscmd()
{
	SystemCommand<void>* cmd =
		new SystemCommand<void>(boost::bind(&LobbyService::reset_lobby, this, this));
	this->regist_syscmd(cmd);
}

LobbyUser* LobbyService::get_user_byslot(int slot)
{
	return 0;
}

LobbyUser* LobbyService::get_user_byslot(int slot, S_INT_64 token)
{
	LobbyUser *p = get_user_byslot(slot);
	if (p == 0)
		return 0;

	if (p->is_samesession(token) == false)
		return 0;

	return p;
}

LobbyUser* LobbyService::get_userbyslot_from_msg(NetProtocol* msg)
{
	//return get_user_byslot(msg->head_.get_token_slot());
	return 0;
}

LobbyUser* LobbyService::get_userofsame_from_msg(NetProtocol* msg)
{
	S_INT_64 uid = 0, tks = 0;
	uid = msg->head_.get_role_iid();
	tks = msg->head_.get_token_token();
	int slot = msg->head_.get_token_slot();

	LobbyUser *puser = get_user_byslot(slot);
	if (puser == 0)
		return 0;
	if (puser->get_user_iid() != uid || puser->is_samesession(tks) == false)
		return 0;
	return puser;
}

LobbyUser* LobbyService::get_userofsame_from_x(S_INT_64 uid, S_INT_64 token)
{
	int slot = 0;

	LobbyUser *puser = get_user_byslot(slot);
	if (puser == 0)
		return 0;
	if (puser->get_user_iid() != uid || puser->is_samesession(token) == false)
		return 0;
	return puser;
}