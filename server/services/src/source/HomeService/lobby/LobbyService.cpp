#include "lobby/LobbyService.h"

#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

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

void LobbyService::init_lobby(int pindex, int nums, std::vector<LobbyUser*>& lus)
{
	this->piece_index_ = pindex;
	this->piece_start_ = pindex * nums;
	this->piece_end_ = piece_start_ + lus.size() - 1;

	this->users_.clear();
	
	for (int ii = 0; ii < lus.size(); ++ii)
	{
		lus[ii]->set_context(this);
		users_.push_back(lus[ii]);
	}

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
	if (slot < piece_start_ || slot > piece_end_)
		return 0;

	return users_[slot - piece_start_];
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

LobbyUser* LobbyService::get_userbyslot_from_msg(BasicProtocol* msg)
{
	int slot = 0;
	if (ProtoUtil::get_slot_from_tokenx(msg, slot) == false)
		return 0;

	return get_user_byslot(slot);
}

LobbyUser* LobbyService::get_userofsame_from_msg(BasicProtocol* msg)
{
	S_INT_64 uid = 0, tks = 0;
	int slot = 0;
	if (ProtoUtil::get_stuidtkm_from_tokenx(msg, slot, uid, tks) == false)
		return 0;

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
	ProtoTokenUtil::parse_usertoken2(token, slot);

	LobbyUser *puser = get_user_byslot(slot);
	if (puser == 0)
		return 0;
	if (puser->get_user_iid() != uid || puser->is_samesession(token) == false)
		return 0;
	return puser;
}