// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "network/FightRouterSession.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/system/CommandBase.h>

#include <FightRouterApp.h>

FightRouterSession::FightRouterSession():parent_(0)
,state_(SessionState_None)
{
	session_.reset(new NetSession( 0, 0, this));
}

FightRouterSession::~FightRouterSession()
{
}

void FightRouterSession::send_protocol(NetProtocol* pro)
{
	session_->send_protocol(pro);
}

NetProtocol* FightRouterSession::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	NetProtocol* pro = new NetProtocol(ntf);

	return pro;
}

void FightRouterSession::on_connect_lost_netthread()
{
	if (is_auth() && parent_)
		parent_->on_connect_lost_netthread();
	else
	{
		SystemCommand<FightRouterSession>* pcmd = new SystemCommand<FightRouterSession>(
			boost::bind(&FightRouterApp::on_connection_timeout, &svrApp, this));
		svrApp.regist_syscmd(pcmd);
	}
}

void FightRouterSession::on_recv_protocol_netthread(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&FightRouterApp::on_mth_servicebindservice_req, &svrApp,
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

void FightRouterSession::reset()
{
	state_ = SessionState_None;
	session_->force_reset();
}

void FightRouterSession::force_close()
{
	session_->force_reset();
}

void FightRouterSession::prepare()
{
	state_ = SessionState_Initialing;
	parent_ = 0;

	auth_timestamp_ = OSSystem::mOS->GetTimestamp();
}

void FightRouterSession::auth()
{
	state_ = SessionState_Authed;
}

void FightRouterSession::update(u64 tnow)
{
	if (state_ == SessionState_Initialing)
	{
		//must finish auth on the 15 seconds, if not, system will force cut conneciton
		if (auth_timestamp_ + 1000 * 15 < tnow)
		{
			state_ = SessionState_Closing;
			SystemCommand<FightRouterSession>* pcmd = new SystemCommand<FightRouterSession>(
				boost::bind(&FightRouterApp::on_connection_timeout, &svrApp, this));
			svrApp.regist_syscmd(pcmd);
		}
	}
	else if(is_auth())
	{
		heart_beat();
	}
}
