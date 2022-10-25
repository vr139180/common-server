#include "network/ServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

ServiceLinkFrom::ServiceLinkFrom():NetLinkFromBase<EurekaSession>()
, node_(0)
{
	this->init_protocolhead();
}

void ServiceLinkFrom::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
}

void ServiceLinkFrom::set_node(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	if( pnode != 0)
		s_head_.to_type_ = (S_INT_8)pnode->type;
}

void ServiceLinkFrom::send_to_service(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	NetLinkFromBase<EurekaSession>::send_protocol(pro);
}

void ServiceLinkFrom::reset()
{
	node_.reset();
	NetLinkFromBase<EurekaSession>::reset();
}

void ServiceLinkFrom::force_linkclose()
{
	force_close();
}

void ServiceLinkFrom::on_connect_lost_netthread()
{
	SystemCommand<ServiceLinkFrom>* cmd = new SystemCommand<ServiceLinkFrom>( boost::bind(
			&ServiceRegisterCtrl::on_mth_disconnected_with_service, svrApp.get_servicectrl(), this));
	svrApp.regist_syscmd( cmd);
}

void ServiceLinkFrom::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	//需要主线程处理的，在这里明确定义处理函数
	if (pro->get_msg() == ERK_PROTYPE::ERK_SERVICEREGIST_CONFIRM)
	{
		NETCMD_FUN_MAP fun = boost::bind(&ServiceRegisterCtrl::on_mth_serviceregist_confirm, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (pro->get_msg() == ERK_PROTYPE::ERK_EUREKA_SYNC)
	{
		Erk_Eureka_sync* syn = dynamic_cast<Erk_Eureka_sync*>(pro->msg_);
		syn->set_myiid(this->get_iid());

		NETCMD_FUN_MAP fun = boost::bind(&EurekaClusterCtrl::on_service_eureka_sync, svrApp.get_eurekactrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (pro->get_msg() == ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ)
	{
		NETCMD_FUN_MAP fun = boost::bind(&ServiceRegisterCtrl::on_mth_servicesubscribe_req, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else
	{
		//缺省处理都在servie线程池中进行
		NETCMD_FUN_MAP fun = boost::bind(
			&ServiceRegisterCtrl::NetProcessMessage, svrApp.get_servicectrl(), boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		svrApp.regist_netcmd(pcmd);
	}
}

void ServiceLinkFrom::registinfo_tolog( bool bregist)
{
	if (bregist)
	{
		logInfo(out_runtime, ">>>>>> Service[iid:%ld type:%d ip:%s port:%d] regist to me(sEureka)",
			node_->iid, node_->type, node_->ip.c_str(), node_->port);
	}
	else 
	{
		logInfo(out_runtime, "<<<<<< Service[iid:%ld type:%d ip:%s port:%d] disconnect from me(sEureka)",
			node_->iid, node_->type, node_->ip.c_str(), node_->port);
	}
}
