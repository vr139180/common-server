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

#include "network/FightRouterLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

FightRouterLinkFrom::FightRouterLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

FightRouterLinkFrom::~FightRouterLinkFrom()
{
}

void FightRouterLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_FIGHTROUTER;
}

void FightRouterLinkFrom::force_linkclose()
{
	force_close();
}

void FightRouterLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<FightRouterLinkFrom>* cmd =
		new SystemCommand<FightRouterLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_fightrouter, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void FightRouterLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	S_UINT_16 msgid = pro->get_msg();
	NETSERVICE_TYPE to = (NETSERVICE_TYPE)pro->get_to();
	if (to == NETSERVICE_TYPE::ERK_SERVICE_HOME)
	{
		logDebug(out_runtime, "recv msg:%d from fightrouter to home", msgid);
		svrApp.router_to_home(pro);
	}
}

void FightRouterLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "FightRouter[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "FightRouter[%d] disconnect from me(DataRouter)", get_iid());
}

void FightRouterLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
