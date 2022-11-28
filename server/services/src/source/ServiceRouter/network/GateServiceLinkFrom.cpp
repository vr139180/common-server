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

#include "chatmodule/ChatModule.h"
#include "mailmodule/MailModule.h"
#include "friends/FriendModule.h"

#include "ServiceRouterApp.h"

GateServiceLinkFrom::GateServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

GateServiceLinkFrom::~GateServiceLinkFrom()
{
}

void GateServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_SVRROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_GATE;
}

void GateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &ServiceRouterApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GateServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	std::unique_ptr<NetProtocol> p_msg(pro);

	if(msgid > PRO::CHAT_PROTYPE::CHAT_MSG_BEGIN && msgid < PRO::CHAT_PROTYPE::CHAT_MSG_END)
	{
		ChatModule::instance().process_chat_msg( p_msg.release());
	}
	else if (msgid > PRO::MAIL_PROTYPE::MAIL_MSG_BEGIN && msgid < PRO::MAIL_PROTYPE::MAIL_MSGALL_END)
	{
		MailModule::instance().process_mail_msg( p_msg.release());
	}
	else if (msgid > PRO::FRIEND_PROTYPE::FRIEND_MSG_BEGIN && msgid < PRO::FRIEND_PROTYPE::FRIEND_MSGALL_END)
	{
		FriendModule::instance().process_friend_msg( p_msg.release());
	}
}

void GateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GateService[%d] regist to me(ServiceRouter)", get_iid());
	else
		logInfo( out_runtime, "GateService[%d] disconnect from me(ServiceRouter)", get_iid());
}

void GateServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}