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

#include "network/FriendServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "friends/FriendModule.h"

#include "ServiceRouterApp.h"

FriendServiceLinkFrom::FriendServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

FriendServiceLinkFrom::~FriendServiceLinkFrom()
{
}

void FriendServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_SVRROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_FRIEND;
}

void FriendServiceLinkFrom::force_linkclose()
{
	force_close();
}

void FriendServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<FriendServiceLinkFrom>* cmd =
		new SystemCommand<FriendServiceLinkFrom>( boost::bind( &ServiceRouterApp::on_disconnected_with_frdservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void FriendServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	std::unique_ptr<NetProtocol> p_msg(pro);
	
	if (msgid == PRO::FRIEND_PROTYPE::FRD_FRIENDCHANGEOTHER_NTF)
	{
		//ϵͳ�ʼ����ͳɹ�֮����route�㲥֪ͨ����mail �������µ�ϵͳ�ʼ�
		//PRO::Frd_FriendChangeOther_ntf* ack = dynamic_cast<PRO::Frd_FriendChangeOther_ntf*>(pro->msg_);
		svrApp.send_protocal_to_friend(p_msg.release());
	}
	else
	{
		S_INT_64 gateid = pro->head_.get_token_gateiid();
		svrApp.send_protocal_to_gate(gateid, p_msg.release());
	}
}

void FriendServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "FriendService[%d] regist to me(ServiceRouter)", get_iid());
	else
		logInfo( out_runtime, "FriendService[%d] disconnect from me(ServiceRouter)", get_iid());
}

void FriendServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}