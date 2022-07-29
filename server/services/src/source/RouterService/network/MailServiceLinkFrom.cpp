#include "network/MailServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include "RouterServiceApp.h"

void MailServiceLinkFrom::force_linkclose()
{
	force_close();
}

void MailServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<MailServiceLinkFrom>* cmd =
		new SystemCommand<MailServiceLinkFrom>( boost::bind( &RouterServiceApp::on_disconnected_with_mailservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void MailServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	
	if (proiid == PRO::MAIL_PROTYPE::MAIL_SYSTEMMAIL_ACK)
	{
		//系统邮件发送成功之后，在route广播通知各个mail 更新最新的系统邮件
		PRO::Mail_SystemMail_ack* ack = dynamic_cast<PRO::Mail_SystemMail_ack*>(pro);
		if (ack->result() == 0)
		{
			PRO::Mail_SystemMail_ntf ntf;
			ntf.set_mailiid( ack->mail().iid());
			svrApp.broad_protocal_to_mails<PRO::Mail_SystemMail_ntf>(&ntf);
		}
	}
	else
	{
		S_INT_64 gateid = 0;
		if (ProtoUtil::get_gate_from_token(pro, gateid))
		{
			svrApp.send_protocal_to_gate(gateid, p_msg.release());
		}
	}
}

void MailServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "MailService[%d] regist to me(RouterService)", get_iid());
	else
		logInfo( out_runtime, "MailService[%d] disconnect from me(RouterService)", get_iid());
}

int MailServiceLinkFrom::get_mailhash()
{
	std::string str = this->get_ext_bykey(MAILSVR_MAILHASH_EXT);
	if (str == "")
		return -1;

	return ShareUtil::atoi(str.c_str());
}
