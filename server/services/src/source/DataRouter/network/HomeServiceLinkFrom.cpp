#include "network/HomeServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

HomeServiceLinkFrom::HomeServiceLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

HomeServiceLinkFrom::~HomeServiceLinkFrom()
{
}

void HomeServiceLinkFrom::init_protocolhead()
{
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_HOME;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
	s_head_.token_gidrid_ = 0;
	s_head_.token_slottoken_ = 0;
}

void HomeServiceLinkFrom::force_linkclose()
{
	force_close();
}

void HomeServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<HomeServiceLinkFrom>* cmd =
		new SystemCommand<HomeServiceLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_homeservice, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void HomeServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
}

void HomeServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "HomeService[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "HomeService[%d] disconnect from me(DataRouter)", get_iid());
}

void HomeServiceLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
