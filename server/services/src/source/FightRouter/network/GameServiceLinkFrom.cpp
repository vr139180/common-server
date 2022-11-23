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

#include "network/GameServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "FightRouterApp.h"

GameServiceLinkFrom::GameServiceLinkFrom() :NetLinkFromBase<FightRouterSession>()
{
	this->init_protocolhead();
}

GameServiceLinkFrom::~GameServiceLinkFrom()
{
}

void GameServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_FIGHTROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_GAME;
}

void GameServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GameServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GameServiceLinkFrom>* cmd =
		new SystemCommand<GameServiceLinkFrom>( boost::bind( &FightRouterApp::on_disconnected_with_gameservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GameServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	if (pro->get_to() == (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE) {
		svrApp.router_to_gate(p_msg.release());
	}
}

void GameServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GameService[%d] regist to me(FightRouter)", get_iid());
	else
		logInfo( out_runtime, "GameService[%d] disconnect from me(FightRouter)", get_iid());
}

void GameServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
