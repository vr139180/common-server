#include "network/RouterSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include "ServiceRouterApp.h"

RouterSession::RouterSession():parent_(0)
,state_(SessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

RouterSession::~RouterSession()
{
}

void RouterSession::send_protocol(BasicProtocol* pro)
{
	session_->send_protocol(pro);
}

BasicProtocol* RouterSession::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}

void RouterSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<RouterSession>* pcmd = new SystemCommand<RouterSession>(
			boost::bind(&ServiceRouterApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void RouterSession::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&ServiceRouterApp::on_mth_servicebindservice_req, &svrApp,
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
	}
	else
	{
		std::unique_ptr<BasicProtocol> p_msg(pro);
		if (is_auth() && parent_)
			parent_->on_recv_protocol_netthread(proiid, p_msg.release());
	}
}

void RouterSession::reset()
{
	state_ = SessionState_None;
	session_->force_reset();
}

void RouterSession::force_close()
{
	session_->force_reset();
}

void RouterSession::prepare()
{
	state_ = SessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void RouterSession::auth()
{
	state_ = SessionState_Authed;
}

void RouterSession::update(u64 tnow)
{
	if (state_ == SessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 15 < tnow)
		{
			state_ = SessionState_Closing;
			SystemCommand<RouterSession>* pcmd = new SystemCommand<RouterSession>(
				boost::bind(&ServiceRouterApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if(is_auth())
	{
		heart_beat();
	}
}
