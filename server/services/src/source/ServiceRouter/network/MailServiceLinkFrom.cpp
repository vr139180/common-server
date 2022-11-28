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

#include "network/MailServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "ServiceRouterApp.h"

MailServiceLinkFrom::MailServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

MailServiceLinkFrom::~MailServiceLinkFrom()
{
}

void MailServiceLinkFrom::init_protocolhead()
{
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_SVRROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_MAIL;
}

void MailServiceLinkFrom::force_linkclose()
{
	force_close();
}

void MailServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<MailServiceLinkFrom>* cmd =
		new SystemCommand<MailServiceLinkFrom>( boost::bind( &ServiceRouterApp::on_disconnected_with_mailservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void MailServiceLinkFrom::on_recv_protocol_netthread(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	std::unique_ptr<NetProtocol> p_msg(pro);
	
	if (msgid == PRO::MAIL_PROTYPE::MAIL_SYSTEMMAIL_ACK)
	{
		//系统邮件发送成功之后，在route广播通知各个mail 更新最新的系统邮件
		PRO::Mail_SystemMail_ack* ack = dynamic_cast<PRO::Mail_SystemMail_ack*>(pro->msg_);
		if (ack->result() == 0)
		{
			PRO::Mail_SystemMail_ntf ntf;
			ntf.set_mailiid( ack->mail().iid());
			svrApp.broad_protocal_to_mails(&ntf);
		}
	}
	else
	{
		S_INT_64 gateid = pro->head_.get_token_gateiid();
		svrApp.send_protocal_to_gate(gateid, p_msg.release());
	}
}

void MailServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "MailService[%d] regist to me(ServiceRouter)", get_iid());
	else
		logInfo( out_runtime, "MailService[%d] disconnect from me(ServiceRouter)", get_iid());
}

void MailServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}