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

#include "network/ChatServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "ServiceRouterApp.h"

ChatServiceLinkFrom::ChatServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

ChatServiceLinkFrom::~ChatServiceLinkFrom()
{
}

void ChatServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_SVRROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_CHAT;
}

void ChatServiceLinkFrom::force_linkclose()
{
	force_close();
}

void ChatServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<ChatServiceLinkFrom>* cmd =
		new SystemCommand<ChatServiceLinkFrom>( boost::bind( &ServiceRouterApp::on_disconnected_with_chatservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void ChatServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	std::unique_ptr<NetProtocol> p_msg(pro);
	if (msgid == PRO::CHAT_PROTYPE::CHAT_GLOBALMSG_NTF)
	{
		svrApp.broad_protocal_to_gate(pro->msg_);
	}
	else if (msgid == PRO::CHAT_PROTYPE::CHAT_CHATMSG_NTF)
	{
		PRO::Chat_ChatMsg_ntf* ntf = dynamic_cast<PRO::Chat_ChatMsg_ntf*>(pro->msg_);
		S_INT_64 gateid = 0;
		svrApp.send_protocal_to_gate(gateid, p_msg.release());
	}
}

void ChatServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "ChatService[%d] regist to me(RouterService)", get_iid());
	else
		logInfo( out_runtime, "ChatService[%d] disconnect from me(RouterService)", get_iid());
}

void ChatServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
