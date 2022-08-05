#include "network/HomeServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "FightRouterServiceApp.h"

void HomeServiceLinkFrom::force_linkclose()
{
	force_close();
}

void HomeServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<HomeServiceLinkFrom>* cmd =
		new SystemCommand<HomeServiceLinkFrom>( boost::bind( &FightRouterServiceApp::on_disconnected_with_homeservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void HomeServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
}

void HomeServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "HomeService[%d] regist to me(FightRouterService)", sn_);
	else
		logInfo( out_runtime, "HomeService[%d] disconnect from me(FightRouterService)", sn_);
}
