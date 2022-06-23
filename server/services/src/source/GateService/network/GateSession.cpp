#include "network/GateSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/global_const.h>
#include <gameLib/LogExt.h>

#include "player/PlayerChannel.h"
#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

GateSession::GateSession():parent_(0)
,state_(SessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

GateSession::~GateSession()
{
}

void GateSession::send_protocol(BasicProtocol* pro)
{
	session_->send_protocol(pro);
}

BasicProtocol* GateSession::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}

void GateSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<GateSession>* pcmd = new SystemCommand<GateSession>(
			boost::bind(&GateServiceApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void GateSession::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	if (proiid == PRO::USER_PROXYLOGIN_REQ)
	{
		user_login(pro);
	}
	else
	{
		std::unique_ptr<BasicProtocol> p_msg(pro);
		if (is_auth() && parent_)
			parent_->on_recv_protocol_netthread(proiid, p_msg.release());
	}
}

void GateSession::user_login(BasicProtocol* msg)
{
	PRO::User_ProxyLogin_req* req = dynamic_cast<PRO::User_ProxyLogin_req*>(msg);
	logDebug(out_net, "user login request slot:%d utoken:%lld", req->slot(),req->proxytoken());

	if (req->slot() < 0 || req->slot() >= GATEHOME_GROUP_NUM)
	{
		PRO::User_ProxyLogin_ack* ack = new PRO::User_ProxyLogin_ack();
		ack->set_result(1);
		send_protocol(ack);

		delete msg;
		return;
	}

	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(req->slot());

	NETCMD_FUN_MAP3 fun = boost::bind(&PlayerChannel::on_cth_userproxylogin_req, pchannel,
		boost::placeholders::_1, boost::placeholders::_2, this);
	NetCommandV *pcmd = new NetCommandV(msg, fun);

	pchannel->regist_syscmd(pcmd);
}

void GateSession::reset()
{
	state_ = SessionState_None;
	session_->force_reset();
}

void GateSession::force_close()
{
	session_->force_reset();
}

void GateSession::prepare()
{
	state_ = SessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void GateSession::auth()
{
	state_ = SessionState_Authed;
}

void GateSession::update(u64 tnow)
{
	if (state_ == SessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 15 < tnow)
		{
			state_ = SessionState_Closing;
			SystemCommand<GateSession>* pcmd = new SystemCommand<GateSession>(
				boost::bind(&GateServiceApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if (is_auth())
	{
		heart_beat();
	}
}
