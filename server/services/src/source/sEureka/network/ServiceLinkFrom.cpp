#include "network/ServiceLinkFrom.h"

#include <cmsLib/system/CommandBase.h>
#include <cmsLib/base/OSSystem.h>

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

ServiceLinkFrom::ServiceLinkFrom():NetLinkFromBase<EurekaSession>()
, node_(0)
{

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

void ServiceLinkFrom::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);

	//需要主线程处理的，在这里明确定义处理函数
	if (proiid == ERK_PROTYPE::ERK_SERVICEREGIST_CONFIRM)
	{
		NETCMD_FUN_MAP fun = boost::bind(&ServiceRegisterCtrl::on_mth_serviceregist_confirm, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (proiid == ERK_PROTYPE::ERK_EUREKA_SYNC)
	{
		Erk_Eureka_sync* syn = dynamic_cast<Erk_Eureka_sync*>(pro);
		syn->set_myiid(this->get_iid());

		NETCMD_FUN_MAP fun = boost::bind(&EurekaClusterCtrl::on_service_eureka_sync, svrApp.get_eurekactrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (proiid == ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ)
	{
		NETCMD_FUN_MAP fun = boost::bind(&ServiceRegisterCtrl::on_mth_servicesubscribe_req, svrApp.get_servicectrl(),
			boost::placeholders::_1, boost::placeholders::_2);
		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);

		svrApp.regist_syscmd(pcmd);
	}
	else
	{
		//缺省处理都在servie线程池中进行
		NETCMD_FUN_MAP2 fun = boost::bind(
			&ServiceRegisterCtrl::NetProcessMessage, svrApp.get_servicectrl(), 
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun ,(int)proiid);
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
