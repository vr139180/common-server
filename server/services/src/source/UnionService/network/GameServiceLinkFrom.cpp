#include "network/GameServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "UnionServiceApp.h"

void GameServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GameServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GameServiceLinkFrom>* cmd =
		new SystemCommand<GameServiceLinkFrom>( boost::bind( &UnionServiceApp::on_disconnected_with_gameservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GameServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
}

void GameServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_boot, "GameService[%d] regist to me(UnionService)", sn_);
	else
		logInfo( out_boot, "GameService[%d] disconnect from me(UnionService)", sn_);
}
