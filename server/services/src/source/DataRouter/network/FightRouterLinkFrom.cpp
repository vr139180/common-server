#include "network/FightRouterLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "DataRouterApp.h"

FightRouterLinkFrom::FightRouterLinkFrom() :NetLinkFromBase<RouterSession>()
{
	this->init_protocolhead();
}

FightRouterLinkFrom::~FightRouterLinkFrom()
{
}

void FightRouterLinkFrom::init_protocolhead()
{
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)PRO::ERK_SERVICE_DATAROUTER;
	s_head_.to_type_ = (S_INT_8)PRO::ERK_SERVICE_FIGHTROUTER;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
	s_head_.token_gidrid_ = 0;
	s_head_.token_slottoken_ = 0;
}

void FightRouterLinkFrom::force_linkclose()
{
	force_close();
}

void FightRouterLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<FightRouterLinkFrom>* cmd =
		new SystemCommand<FightRouterLinkFrom>( boost::bind( &DataRouterApp::on_disconnected_with_fightrouter, &svrApp, this));
	svrApp.regist_syscmd( cmd);
}

void FightRouterLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
}

void FightRouterLinkFrom::registinfo_tolog( bool bregist)
{
	if( bregist)
		logInfo( out_runtime, "FightRouter[%d] regist to me(DataRouter)", get_iid());
	else
		logInfo( out_runtime, "FightRouter[%d] disconnect from me(DataRouter)", get_iid());
}

void FightRouterLinkFrom::send_netprotocol(BasicProtocol* msg)
{
	NetProtocol *pro = new NetProtocol(get_protocolhead(), msg);

	this->send_protocol(pro);
}
