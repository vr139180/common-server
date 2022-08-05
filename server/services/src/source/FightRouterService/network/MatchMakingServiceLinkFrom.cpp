#include "network/MatchMakingServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "FightRouterServiceApp.h"

void MatchMakingServiceLinkFrom::force_linkclose()
{
	force_close();
}

void MatchMakingServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<MatchMakingServiceLinkFrom>* cmd =
		new SystemCommand<MatchMakingServiceLinkFrom>( boost::bind( &FightRouterServiceApp::on_disconnected_with_matchmakingservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void MatchMakingServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
}

void MatchMakingServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "MatchMaking Service[%d] regist to me(FightRouterService)", sn_);
	else
		logInfo( out_runtime, "MatchMaking Service[%d] disconnect from me(FightRouterService)", sn_);
}
