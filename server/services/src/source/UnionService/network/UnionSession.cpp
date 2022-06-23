#include "network/UnionSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include <UnionServiceApp.h>

UnionSession::UnionSession():parent_(0)
,state_(SessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

UnionSession::~UnionSession()
{
}

BasicProtocol* UnionSession::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}

void UnionSession::send_protocol(BasicProtocol* pro)
{
	session_->send_protocol(pro);
}

void UnionSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<UnionSession>* pcmd = new SystemCommand<UnionSession>(
			boost::bind(&UnionServiceApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void UnionSession::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	{
		std::unique_ptr<BasicProtocol> p_msg(pro);
		if (is_auth() && parent_)
			parent_->on_recv_protocol_netthread(proiid, p_msg.release());
	}
}

void UnionSession::reset()
{
	state_ = SessionState_None;
	session_->force_reset();
}

void UnionSession::force_close()
{
	session_->force_reset();
}

void UnionSession::prepare()
{
	state_ = SessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void UnionSession::auth()
{
	state_ = SessionState_Authed;
}

void UnionSession::update(u64 tnow)
{
	if (state_ == SessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 15 < tnow)
		{
			state_ = SessionState_Closing;
			SystemCommand<UnionSession>* pcmd = new SystemCommand<UnionSession>(
				boost::bind(&UnionServiceApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if (is_auth())
	{
		heart_beat();
	}
}
