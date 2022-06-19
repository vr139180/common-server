#include "player/PlayerChannel.h"

#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <gameLib/LogExt.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

PlayerChannel::PlayerChannel() :VirtualMainThread()
,piece_index_(-1)
, piece_start_(0)
, piece_end_(0)
,parent_(0)
,online_user_nums_(0)
{
}

PlayerChannel::~PlayerChannel()
{
}

void PlayerChannel::thread_worker()
{
	OSSystem::mOS->UpdateNowTick();
	u64 st = OSSystem::mOS->GetTicks();

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
		std::auto_ptr<CommandBase> a_pcmd(pCmd);

		if (pCmd == 0)
		{
			sleepstep = loop_num_;
			continue;
		}

		pCmd->run();
	}
}

void PlayerChannel::init_channel(GamePlayerCtrl* p, int pindex, int nums, std::vector<GamePlayer*>& lus)
{
	this->parent_ = p;
	this->piece_index_ = pindex;
	this->piece_start_ = pindex * nums;
	this->piece_end_ = piece_start_ + lus.size() - 1;

	this->users_.clear();

	this->online_user_nums_ = 0;

	online_users_.clear();
	for (int ii = 0; ii < lus.size(); ++ii)
		users_.push_back(lus[ii]);

	this->InitNetMessage();
}

void PlayerChannel::reset_channel(void*)
{
	this->online_user_nums_ = 0;

	online_users_.clear();
}

void PlayerChannel::reset_syscmd()
{
	SystemCommand<void>* cmd =
		new SystemCommand<void>(boost::bind(&PlayerChannel::reset_channel, this, this));
	this->regist_syscmd(cmd);
}


GamePlayer* PlayerChannel::ask_free_slot(int slot, S_INT_64 token, S_INT_64 uid)
{
	GamePlayer* p = get_player_byslot(slot);

	//init before used
	p->preuse(token, uid, EurekaClusterClient::instance().get_myiid());

	return p;
}

GamePlayer* PlayerChannel::get_player_byslot(int slot)
{
	if (slot < piece_start_ || slot > piece_end_)
		return 0;

	return users_[slot-piece_start_];
}

GamePlayer* PlayerChannel::get_player_byslot(int slot, S_INT_64 token)
{
	GamePlayer *p = get_player_byslot(slot);
	if (p == 0)
		return 0;

	if (p->is_samesession(token) == false)
		return 0;

	return p;
}

GamePlayer* PlayerChannel::get_player_frommsg(BasicProtocol* msg)
{
	S_INT_64 uid = 0, tk = 0;
	int slot = 0;
	if (ProtoUtil::get_stuidtkm_from_tokenx(msg, slot, uid, tk) == false)
		return 0;

	GamePlayer *user = get_player_byslot(slot);
	if (user == 0)
		return 0;
	if (user->is_samesession(uid, tk) == false)
		return 0;
	return user;
}

void PlayerChannel::force_pc_close_player(GamePlayer* puser)
{
	GateSession *psession = puser->get_session();
	if (psession == 0)
		return;

	online_users_.erase(puser->get_iid());
	online_user_nums_ = online_users_.size();

	puser->set_session(0);
	puser->registinfo_tolog(false);

	psession->set_netlinkbase(0);

	puser->reset();

	//¶Ï¿ªÓ³Éä¹ØÏµ
	parent_->return_slot_to_free(puser->get_userslot());

	svrApp.on_disconnected_with_player(psession);
}

void PlayerChannel::send_msg_to_player(BasicProtocol* msg, int slot)
{
	GamePlayer *p = get_player_byslot(slot);
	if (p == 0)
	{
		delete msg;
		return;
	}

	p->send_protocol(msg);
}