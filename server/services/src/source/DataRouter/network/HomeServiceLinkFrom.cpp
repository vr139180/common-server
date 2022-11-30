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

#include "network/HomeServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

HomeServiceLinkFrom::HomeServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

HomeServiceLinkFrom::~HomeServiceLinkFrom()
{
}

void HomeServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_HOME;
}

void HomeServiceLinkFrom::force_linkclose()
{
	force_close();
}

void HomeServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<HomeServiceLinkFrom>* cmd =
		new SystemCommand<HomeServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_homeservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void HomeServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	NETSERVICE_TYPE to = (NETSERVICE_TYPE)pro->get_to();
	if (to == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		svrApp.router_to_gate(p_msg.release());
	}
	else if (to == NETSERVICE_TYPE::ERK_SERVICE_GAME)
	{
		svrApp.router_to_game(p_msg.release());
	}
}

void HomeServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "HomeService[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "HomeService[%d] disconnect from me(DataRouter)", get_iid());
}

void HomeServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
