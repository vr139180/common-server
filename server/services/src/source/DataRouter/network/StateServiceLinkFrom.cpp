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

#include "network/StateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

StateServiceLinkFrom::StateServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

StateServiceLinkFrom::~StateServiceLinkFrom()
{
}

void StateServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_STATE;
}

void StateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void StateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_stateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void StateServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	NETSERVICE_TYPE gto = (NETSERVICE_TYPE)pro->get_to();
	if (gto == NETSERVICE_TYPE::ERK_SERVICE_GATE) {
		svrApp.router_to_gate(p_msg.release());
	}
	else if (gto == NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER) {
		//to 为datarouter的都是需要特殊处理的
		to_datarouter_dealwith(p_msg.release());
	}
}

void StateServiceLinkFrom::to_datarouter_dealwith(NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	int msgid = (int)pro->get_msg();
	if (msgid == PRO::USER_PROTYPE::USER_LOGOUT_NTF)
	{
		//logout需要同步给 gate+game
		PRO::User_Logout_ntf* ntf = dynamic_cast<PRO::User_Logout_ntf*>(pro->msg_);
		if (ntf->gameid() > 0)
		{
			NetProtocol* np = pro->clone();
			SProtocolHead& head = np->write_head();
			head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_STATE;

			svrApp.router_to_game(np);
		}

		svrApp.router_to_gate(p_msg.release());
	}
}

void StateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "StateService[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "StateService[%d] disconnect from me(DataRouter)", get_iid());
}

void StateServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}