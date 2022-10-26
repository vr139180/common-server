#include "network/StateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

void StateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void StateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void StateServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
}

void StateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GateService[%d] regist to me(RouterSession)", get_iid());
	else
		logInfo( out_runtime, "GateService[%d] disconnect from me(RouterSession)", get_iid());
}
