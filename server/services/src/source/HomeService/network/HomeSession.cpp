#include "network/HomeSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include <HomeServiceApp.h>

HomeSession::HomeSession():parent_(0)
,state_(HomeSessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

HomeSession::~HomeSession()
{
}

void HomeSession::send_protocol(BasicProtocol* pro)
{
	session_->send_protocol(pro);
}

BasicProtocol* HomeSession::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}

void HomeSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<HomeSession>* pcmd = new SystemCommand<HomeSession>(
			boost::bind(&HomeServiceApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void HomeSession::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&HomeServiceApp::on_mth_servicebindservice_req, &svrApp,
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

void HomeSession::reset()
{
	state_ = HomeSessionState_None;
	session_->force_reset();
}

void HomeSession::force_close()
{
	session_->force_reset();
}

void HomeSession::prepare()
{
	state_ = HomeSessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void HomeSession::auth()
{
	state_ = HomeSessionState_Authed;
}

void HomeSession::update(u64 tnow)
{
	if (state_ == HomeSessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 15 < tnow)
		{
			state_ = HomeSessionState_Closing;
			SystemCommand<HomeSession>* pcmd = new SystemCommand<HomeSession>(boost::bind(&HomeServiceApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if (is_auth())
	{
		heart_beat();
	}
}
