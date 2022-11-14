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

#include "network/LoginServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

LoginServiceLinkFrom::LoginServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

LoginServiceLinkFrom::~LoginServiceLinkFrom()
{
}

void LoginServiceLinkFrom::init_protocolhead()
{
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_LOGIN;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
	s_head_.token_gidrid_ = 0;
	s_head_.token_slottoken_ = 0;
}

void LoginServiceLinkFrom::force_linkclose()
{
	force_close();
}

void LoginServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<LoginServiceLinkFrom>* cmd =
		new SystemCommand<LoginServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_loginservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void LoginServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
}

void LoginServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "LoginService[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "LoginService[%d] disconnect from me(DataRouter)", get_iid());
}

void LoginServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}