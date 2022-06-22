#include "network/EurekaLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

EurekaLinkFrom::EurekaLinkFrom():NetLinkFromBase<EurekaSession>()
,node_( 0)
{
}

EurekaLinkFrom::EurekaLinkFrom(EurekaNodeInfo* pnode) : NetLinkFromBase<EurekaSession>()
,node_( pnode)
{
}

void EurekaLinkFrom::reset()
{
	node_.reset();
	NetLinkFromBase<EurekaSession>::reset();
}

void EurekaLinkFrom::force_linkclose()
{
	force_close();
}

void EurekaLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<EurekaLinkFrom>* cmd = new SystemCommand<EurekaLinkFrom>( 
			boost::bind( &EurekaClusterCtrl::on_disconnected_with_linkfrom, svrApp.get_eurekactrl(), this));
	svrApp.regist_syscmd( cmd);
}

void EurekaLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);

	NETCMD_FUN_MAP2 fun = boost::bind(&EurekaClusterCtrl::NetProcessMessage, svrApp.get_eurekactrl(), 
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

	NetCommand *pcmd = new NetCommand(p_msg.release(), fun, (int)proiid);
	svrApp.regist_syscmd(pcmd);
}

void EurekaLinkFrom::registinfo_tolog( bool bregist)
{
	if (node_.get() == 0)
		return;

	if( bregist)
		logInfo( out_net, ">>>>>> sEureka[%d] regist to me(sEureka)", node_->iid);
	else
		logInfo(out_net, "<<<<<< sEureka[%d] disconnect from me(sEureka)", node_->iid);
}

void EurekaLinkFrom::heart_beat()
{
	NetLinkFromBase<EurekaSession>::heart_beat();
}
