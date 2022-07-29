#include "network/GateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>

#include "lobby/LobbyService.h"
#include "HomeServiceApp.h"

void GateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &HomeServiceApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GateServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::ERK_PROTYPE::SVR_GATEBINDHOME_CONFIRM)
	{
		NETCMD_FUN_MAP fun = boost::bind(
			&HomeServiceApp::on_mth_gatebindhome_confirm, &svrApp, boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		svrApp.regist_syscmd(pcmd);
	}
	else if (proiid == PRO::ERK_PROTYPE::RES_PROXYUSERSLOT_ACK)
	{
		PRO::Res_ProxyUserSlot_ack* ack = dynamic_cast<PRO::Res_ProxyUserSlot_ack*>(pro);

		logDebug(out_runtime, "recv gate service userproxy slot response userid:%lld token:%s and route to res service", 
			ack->user_iid(), ack->proxytoken().c_str());
		svrApp.send_protocol_to_res(p_msg.release());
	}
	else if (proiid == PRO::ERK_PROTYPE::RES_SYNCGATESLOT_NTF)
	{
		logDebug(out_runtime, "recv gate service sync gate slot ntf, and route to res service");
		svrApp.send_protocol_to_res(p_msg.release());
	}
	else
	{
		int slot = 0;
		if (ProtoUtil::get_slot_from_tokenx(pro, slot))
		{
			LobbyService* pls = svrApp.get_lobbysvr_by_slot(slot);
			if (pls != 0)
			{
				NETCMD_FUN_MAP2 fun = boost::bind(&LobbyService::NetProcessMessage, pls,
					boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

				NetCommand *pcmd = new NetCommand(p_msg.release(), fun, (int)proiid);
				pls->regist_netcmd(pcmd);
			}
		}

	}
	/*
	NETCMD_FUN_MAP fun = boost::bind( &HomeServiceApp::NetProcessMessage, &svrApp, (int)proiid, boost::placeholders::_1, boost::placeholders::_2);

	NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
	svrApp.regist_netcmd(pcmd);
	*/
}

void GateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GateService[%d] regist to me(HomeService)", get_iid());
	else
		logInfo( out_runtime, "GateService[%d] disconnect from me(HomeService)", get_iid());
}
