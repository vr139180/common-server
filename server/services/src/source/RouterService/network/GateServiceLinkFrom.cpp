#include "network/GateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "chatmodule/ChatModule.h"
#include "mailmodule/MailModule.h"
#include "friends/FriendModule.h"

#include "RouterServiceApp.h"

void GateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &RouterServiceApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GateServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);

	if( proiid > PRO::CHAT_PROTYPE::CHAT_MSG_BEGIN && proiid < PRO::CHAT_PROTYPE::CHAT_MSG_END)
	{
		ChatModule::instance().process_chat_msg(proiid, p_msg.release());
	}
	else if (proiid > PRO::MAIL_PROTYPE::MAIL_MSG_BEGIN && proiid < PRO::MAIL_PROTYPE::MAIL_MSGALL_END)
	{
		MailModule::instance().process_mail_msg(proiid, p_msg.release());
	}
	else if (proiid > PRO::FRIEND_PROTYPE::FRIEND_MSG_BEGIN && proiid < PRO::FRIEND_PROTYPE::FRIEND_MSGALL_END)
	{
		FriendModule::instance().process_friend_msg(proiid, p_msg.release());
	}
}

void GateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_boot, "GateService[%d] regist to me(RouterSession)", get_iid());
	else
		logInfo( out_boot, "GateService[%d] disconnect from me(RouterSession)", get_iid());
}
