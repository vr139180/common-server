#include "network/GateServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

void GateServiceLinkFrom::force_linkclose()
{
	force_close();
}

void GateServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<GateServiceLinkFrom>* cmd =
		new SystemCommand<GateServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_gateservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void GateServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
}

void GateServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "GateService[%d] regist to me(RouterSession)", get_iid());
	else
		logInfo( out_runtime, "GateService[%d] disconnect from me(RouterSession)", get_iid());
}
