#include "network/ChatServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "RouterServiceApp.h"

void ChatServiceLinkFrom::force_linkclose()
{
	force_close();
}

void ChatServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<ChatServiceLinkFrom>* cmd =
		new SystemCommand<ChatServiceLinkFrom>( boost::bind( &RouterServiceApp::on_disconnected_with_chatservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void ChatServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::CHAT_PROTYPE::CHAT_GLOBALMSG_NTF)
	{
		PRO::Chat_GlobalMsg_ntf *ntf = dynamic_cast<PRO::Chat_GlobalMsg_ntf*>(pro);
		svrApp.broad_protocal_to_gate< PRO::Chat_GlobalMsg_ntf>(ntf);
	}
	else if (proiid == PRO::CHAT_PROTYPE::CHAT_CHATMSG_NTF)
	{
		PRO::Chat_ChatMsg_ntf* ntf = dynamic_cast<PRO::Chat_ChatMsg_ntf*>(pro);
		S_INT_64 gateid = 0;
		ProtoTokenUtil::parse_usergate3(ntf->utoken().giduid(), gateid);
		svrApp.send_protocal_to_gate(gateid, p_msg.release());
	}
}

void ChatServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_boot, "ChatService[%d] regist to me(RouterService)", get_iid());
	else
		logInfo( out_boot, "ChatService[%d] disconnect from me(RouterService)", get_iid());
}

int ChatServiceLinkFrom::get_chathash()
{
	std::string str = this->get_ext_bykey(CHATSVR_CHATHASH_EXT);
	if (str == "")
		return -1;

	return ShareUtil::atoi(str.c_str());
}
