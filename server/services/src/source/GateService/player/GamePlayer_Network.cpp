#include "player/GamePlayer.h"

#include <cmsLib/base/OSSystem.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

void GamePlayer::on_connect_lost_netthread()
{
	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot_);

	SystemCommand<GamePlayer>* cmd =
		new SystemCommand<GamePlayer>(boost::bind(&PlayerChannel::on_disconnected_with_player, pchannel, this));
	pchannel->regist_syscmd(cmd);
}

bool GamePlayer::is_need_unpack_protocol(S_UINT_16 msgid)
{
	return true;
}

void GamePlayer::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::USER_PROXYLOGIN_REQ)
	{
		user_login(pro);
	}
	else if (msgid == PRO::USER_ROLECREATE_REQ || msgid == PRO::USER_ROLESELECT_REQ)
	{
		if( !is_in_rolerange())
			return;

		svrApp.route_to_datarouter( PRO::ERK_SERVICE_HOME, p_msg.release());
	}
	else
	{
		if( !is_roleready())
			return;

		if(msgid > PRO::CHAT_PROTYPE::CHAT_MSG_BEGIN && msgid < PRO::CHAT_PROTYPE::CHAT_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_CHAT, p_msg.release());
			return;
		}
		else if (msgid > PRO::MAIL_PROTYPE::MAIL_MSG_BEGIN && msgid < PRO::MAIL_PROTYPE::MAIL_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_MAIL, p_msg.release());
			return;
		}
		else if (msgid > PRO::FRIEND_PROTYPE::FRIEND_MSG_BEGIN && msgid < PRO::FRIEND_PROTYPE::FRIEND_MSG_END)
		{
			svrApp.route_to_servicerouter(PRO::ERK_SERVICE_FRIEND, p_msg.release());
			return;
		}
		else if (msgid > PRO::TASK_PROTYPE::TASK_MSG_BEGIN && msgid < PRO::TASK_PROTYPE::TASK_MSG_END)
		{
			svrApp.route_to_datarouter(PRO::ERK_SERVICE_HOME, p_msg.release());
			return;
		}
		else //default, route to datarouter
		{
			svrApp.route_to_datarouter(PRO::ERK_SERVICE_HOME, p_msg.release());
		}
	}
}

void GamePlayer::registinfo_tolog(bool bregist)
{
	if (bregist)
	{
		logInfo(out_runtime, "Player[%d] regist to me(GateService)", get_iid());

		//set login flag
		cur_state_ = PlayerState::PlayerState_Loginning;
	}
	else
		logInfo(out_runtime, "Player[%d] disconnect from me(GateService)", get_iid());
}

void GamePlayer::user_login(NetProtocol* msg)
{
	this->cur_state_ = PlayerState::PlayerState_Loginning;

	PRO::User_ProxyLogin_req* req = dynamic_cast<PRO::User_ProxyLogin_req*>(msg->msg_);
	logDebug(out_runtime, "user login request slot:%d utoken:%lld", slot_, req->proxytoken());

	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot_);

	NETCMD_FUN_MAP3 fun = boost::bind(&PlayerChannel::on_cth_userproxylogin_req, pchannel,
		boost::placeholders::_1, boost::placeholders::_2, this);
	NetCommandV *pcmd = new NetCommandV(msg, fun);

	pchannel->regist_syscmd(pcmd);
}
