#include "network/EurekaSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include "cluster/EurekaClusterCtrl.h"
#include <sEurekaApp.h>

EurekaSession::EurekaSession():parent_(0)
,state_(SessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

EurekaSession::~EurekaSession()
{
}

NetProtocol* EurekaSession::get_livekeep_msg()
{
	Svr_LiveTick_ntf* ntf = new Svr_LiveTick_ntf();
	NetProtocol* pro = new NetProtocol(ntf);
	SProtocolHead& head = pro->write_head();
	head.router_balance_ = false;

	return pro;
}

void EurekaSession::send_to_service(const SProtocolHead& head, BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(head, msg);
	session_->send_protocol(pro);
}

void EurekaSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<EurekaSession>* pcmd = new SystemCommand<EurekaSession>(
			boost::bind(&sEurekaApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void EurekaSession::on_recv_protocol_netthread( NetProtocol* pro)
{
	if (pro->get_msg() == ERK_PROTYPE::ERK_EUREKABIND_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&EurekaClusterCtrl::on_eurekabind_req, svrApp.get_eurekactrl(),
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (pro->get_msg() == ERK_PROTYPE::ERK_SERVICEREGIST_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&ServiceRegisterCtrl::on_mth_serviceregist_req, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (pro->get_msg() == ERK_PROTYPE::ERK_SERVICEBIND_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&ServiceRegisterCtrl::on_mth_servicebind_req, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
	}
	else
	{
		std::unique_ptr<NetProtocol> p_msg(pro);
		if (is_auth() && parent_)
			parent_->on_recv_protocol_netthread( p_msg.release());
	}
}

void EurekaSession::reset()
{
	state_ = SessionState_None;
	session_->force_reset();
}

void EurekaSession::force_close()
{
	session_->force_reset();
}

void EurekaSession::prepare()
{
	state_ = SessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = (S_INT_64)OSSystem::mOS->GetTimestamp();
}

void EurekaSession::auth()
{
	state_ = SessionState_Authed;
}

void EurekaSession::update(u64 tnow)
{
	if (state_ == SessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 10 < tnow)
		{
#ifdef EUREKA_DEBUGINFO_ENABLE
			logDebug(out_runtime, "eureka session timeout......");
#endif

			state_ = SessionState_Closing;
			SystemCommand<EurekaSession>* pcmd = new SystemCommand<EurekaSession>(
				boost::bind(&sEurekaApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if (is_auth())
	{
		heart_beat();
	}
}
