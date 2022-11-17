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

#include "network/GateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

GateServiceLinkFrom::GateServiceLinkFrom():NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

GateServiceLinkFrom::~GateServiceLinkFrom()
{
}

void GateServiceLinkFrom::init_protocolhead()
{
	s_head_.router_balance_ = false;
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_GATE;
}

void GateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GateServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::USER_LOGIN_REQ || msgid == PRO::USER_RELOGIN_REQ)
	{
		logDebug(out_runtime, "recv msg:%d from gate to state", msgid);
		svrApp.router_to_state(pro);
	}
	else
	{
		svrApp.router_to_home(pro);
	}
}

void GateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GateService[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "GateService[%d] disconnect from me(DataRouter)", get_iid());
}

void GateServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}