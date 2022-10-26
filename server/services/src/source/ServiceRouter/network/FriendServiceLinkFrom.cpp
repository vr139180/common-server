#include "network/FriendServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>
#include <cmsLib/util/ShareUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "friends/FriendModule.h"

#include "ServiceRouterApp.h"

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

void FriendServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	
	if (proiid == PRO::FRIEND_PROTYPE::FRD_FRIENDCHANGEOTHER_NTF)
	{
		//系统邮件发送成功之后，在route广播通知各个mail 更新最新的系统邮件
		PRO::Frd_FriendChangeOther_ntf* ack = dynamic_cast<PRO::Frd_FriendChangeOther_ntf*>(pro);
		int lind = FriendModule::instance().user_to_frdhash(ack->notify_roleiid());
		svrApp.send_protocal_to_friend(lind, p_msg.release());
	}
	else
	{
		S_INT_64 gateid = 0;
		svrApp.send_protocal_to_gate(gateid, p_msg.release());
	}
}

void FriendServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "FriendService[%d] regist to me(RouterService)", get_iid());
	else
		logInfo( out_runtime, "FriendService[%d] disconnect from me(RouterService)", get_iid());
}

int FriendServiceLinkFrom::get_frdhash()
{
	std::string str = this->get_ext_bykey(FRIENDSVR_FRDHASH_EXT);
	if (str == "")
		return -1;

	return ShareUtil::atoi(str.c_str());
}
