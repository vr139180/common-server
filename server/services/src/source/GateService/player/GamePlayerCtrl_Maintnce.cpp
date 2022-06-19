#include "player/GamePlayerCtrl.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/eureka/EurekaClusterClient.h>
#include <gameLib/LogExt.h>

#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void GamePlayerCtrl::ask_gateslot_sync()
{
	SystemCommand<GamePlayerCtrl>* pcmd = new SystemCommand<GamePlayerCtrl>(
		boost::bind(&GamePlayerCtrl::on_mth_gateslot_sync, this, this));
	svrApp.regist_syscmd(pcmd);
}

void GamePlayerCtrl::on_mth_gateslot_sync(void*)
{
	Res_SyncGateSlot_ntf* ntf = new Res_SyncGateSlot_ntf();
	
	++slot_sequence_;
	ntf->set_syncsequence(slot_sequence_);
	ntf->set_gateiid(EurekaClusterClient::instance().get_myiid());
	ntf->set_homeiid(svrApp.get_homeholder()->get_iid());
	
	{
		ThreadLockWrapper tl(lock_);

		ntf->set_freeslots(free_slot_num_);
	}

	svrApp.send_to_homeservice(ntf);
}

void GamePlayerCtrl::maintnce_proxylogin_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	if (svrApp.is_bindhome() == false)
		return;

	bool bsync = false;
	{
		ThreadLockWrapper tl(lock_);

		for (std::list<GamePlayer*>::iterator iter = wait_auth_slots_queue_.begin(); iter != wait_auth_slots_queue_.end(); )
		{
			GamePlayer *p = (*iter);

			//15s 登陆超时
			if ((p->get_proxystamp() + 15 * 1000) > tnow)
				break;

			iter = wait_auth_slots_queue_.erase(iter);
			free_slots_.insert(p);
			bsync = true;
		}

		if (bsync)
		{
			wait_auth_slot_num_ = wait_auth_slots_queue_.size();
			free_slot_num_ = free_slots_.size();
		}

		//10s 无freeslot请求，则强制同步
		if (last_freeslot_ask_ + 10 * 1000 < tnow)
			bsync = true;
	}

	//如果需要通知，发送通知
	if (bsync)
		on_mth_gateslot_sync(0);
}
