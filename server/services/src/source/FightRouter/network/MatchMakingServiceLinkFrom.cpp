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

#include "network/MatchMakingServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "FightRouterApp.h"

MatchMakingServiceLinkFrom::MatchMakingServiceLinkFrom() :NetLinkFromBase<FightRouterSession>()
{
	this->init_protocolhead();
}

MatchMakingServiceLinkFrom::~MatchMakingServiceLinkFrom()
{
}

void MatchMakingServiceLinkFrom::init_protocolhead()
{
	s_head_.router_balance_ = false;
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_FIGHTROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_MATCHMAKING;
}

void MatchMakingServiceLinkFrom::force_linkclose()
{
	force_close();
}

void MatchMakingServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<MatchMakingServiceLinkFrom>* cmd =
		new SystemCommand<MatchMakingServiceLinkFrom>( boost::bind( &FightRouterApp::on_disconnected_with_matchmakingservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void MatchMakingServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
}

void MatchMakingServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "MatchMaking Service[%d] regist to me(FightRouter)", get_iid());
	else
		logInfo( out_runtime, "MatchMaking Service[%d] disconnect from me(FightRouter)", get_iid());
}

void MatchMakingServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}