#include "network/EurekaLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

EurekaLinkFrom::EurekaLinkFrom():NetLinkFromBase<EurekaSession>()
,node_( 0)
{
	this->init_protocolhead();
}

EurekaLinkFrom::EurekaLinkFrom(EurekaNodeInfo* pnode) : NetLinkFromBase<EurekaSession>()
,node_( pnode)
{
	this->init_protocolhead();
}

void EurekaLinkFrom::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
}

void EurekaLinkFrom::send_to_eureka(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	NetLinkFromBase<EurekaSession>::send_protocol(pro);
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

void EurekaLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	NETCMD_FUN_MAP fun = boost::bind(&EurekaClusterCtrl::NetProcessMessage, svrApp.get_eurekactrl(), 
		boost::placeholders::_1, boost::placeholders::_2);

	NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
	svrApp.regist_syscmd(pcmd);
}

void EurekaLinkFrom::registinfo_tolog( bool bregist)
{
	if (node_.get() == 0)
		return;

	if( bregist)
		logInfo(out_runtime, ">>>>>> sEureka[%d] regist to me(sEureka)", node_->iid);
	else
		logInfo(out_runtime, "<<<<<< sEureka[%d] disconnect from me(sEureka)", node_->iid);
}

void EurekaLinkFrom::heart_beat()
{
	NetLinkFromBase<EurekaSession>::heart_beat();
}
