#include "player/GamePlayer.h"

#include <cmsLib/base/OSSystem.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

void GamePlayer::on_connect_lost_netthread()
{
	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot_);

	SystemCommand<GamePlayer>* cmd =
		new SystemCommand<GamePlayer>(boost::bind(&PlayerChannel::on_disconnected_with_player, pchannel, this));
	pchannel->regist_syscmd(cmd);
}

void GamePlayer::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);

	if (proiid == PRO::USER_ROLECREATE_REQ || proiid == PRO::USER_ROLESELECT_REQ)
	{
		if( !is_in_rolerange())
			return;

		if( set_usertoken( pro))
		{
			svrApp.send_to_homeservice(p_msg.release());
		}
	}
	else
	{
		if( !is_roleready())
			return;

		if(proiid > PRO::CHAT_PROTYPE::CHAT_MSG_BEGIN && proiid < PRO::CHAT_PROTYPE::CHAT_MSG_END)
		{
			//设置用户协议头
			set_usertoken(pro);

			svrApp.send_protocol_to_router(p_msg.release());
			return;
		}
		else if (proiid > PRO::MAIL_PROTYPE::MAIL_MSG_BEGIN && proiid < PRO::MAIL_PROTYPE::MAIL_MSG_END)
		{
			//设置用户协议头
			set_usertoken(pro);

			svrApp.send_protocol_to_router(p_msg.release());
			return;
		}
		else if (proiid > PRO::FRIEND_PROTYPE::FRIEND_MSG_BEGIN && proiid < PRO::FRIEND_PROTYPE::FRIEND_MSG_END)
		{
			//设置用户协议头
			set_usertoken(pro);

			svrApp.send_protocol_to_router(p_msg.release());
			return;
		}
		else if (proiid > PRO::TASK_PROTYPE::TASK_MSG_BEGIN && proiid < PRO::TASK_PROTYPE::TASK_MSG_END)
		{
			//设置用户协议头
			set_usertoken(pro);

			svrApp.send_to_homeservice(p_msg.release());
			return;
		}
		else
		{
			if (set_usertoken(pro))
			{
				svrApp.send_to_homeservice(p_msg.release());
			}
		}

		/*
		//设置缺省协议
		if (ProtoUtil::set_usertoken(pro, get_iid(), proto_token_))
		{
			PlayerChannel* pchannel = GamePlayerCtrl::instance().get_channel_by_slot(this->slot_);
			if (pchannel != 0)
			{
				NETCMD_FUN_MAP2 fun = boost::bind(&PlayerChannel::NetProcessMessage, pchannel,
					boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

				NetCommand *pcmd = new NetCommand(p_msg.release(), fun, (int)proiid);
				pchannel->regist_netcmd(pcmd);
			}
		}
		*/
	}
}

void GamePlayer::registinfo_tolog(bool bregist)
{
	if (bregist)
	{
		logInfo(out_boot, "Player[%d] regist to me(GateService)", get_iid());

		//set login flag
		cur_state_ = PlayerState::PlayerState_Login;
	}
	else
		logInfo(out_boot, "Player[%d] disconnect from me(GateService)", get_iid());
}

