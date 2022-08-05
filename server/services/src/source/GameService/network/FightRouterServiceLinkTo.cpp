#include "network/FightRouterServiceLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <GameServiceApp.h>

FightRouterServiceLinkTo::FightRouterServiceLinkTo() :LinkToBase()
, node_(0)
{
}

FightRouterServiceLinkTo::FightRouterServiceLinkTo(ServiceNodeInfo* pnode) : LinkToBase()
, node_(pnode)
{

}

void FightRouterServiceLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void FightRouterServiceLinkTo::force_linkclose()
{
	force_close();
}

std::string FightRouterServiceLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void FightRouterServiceLinkTo::connect()
{
	if (is_connected() || is_connecting())
		return;

	logInfo(out_runtime, "me(GameService) try to connect to FightRouter Service(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void FightRouterServiceLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_runtime, "------me(GameService) cant connect to FightRouter Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&FightRouterServiceLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void FightRouterServiceLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_runtime, "++++++me(GameService) connected to FightRouter Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&FightRouterServiceLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void FightRouterServiceLinkTo::on_connect_lost_netthread()
{
	SystemCommand<bool>* cmd = new SystemCommand<bool>(
		boost::bind(&FightRouterServiceLinkTo::on_disconnected, this));
	svrApp.regist_syscmd(cmd);
}

void FightRouterServiceLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&FightRouterServiceLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
}

void FightRouterServiceLinkTo::on_connected(bool success)
{
	if (success)
	{
		//ע�ᵽhome
		PRO::Svr_ServiceBindService_req *req = new PRO::Svr_ServiceBindService_req();
		req->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_GAME);
		req->set_myiid(EurekaClusterClient::instance().get_myiid());
		req->set_mytoken(EurekaClusterClient::instance().get_token());
		req->set_toiid(node_->iid);
		req->set_totoken(node_->token);

		this->send_protocol(req);
	}
	else
	{
		logError(out_runtime, "me(GameService) can't connect to FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);

		svrApp.on_disconnected_with_fightrouterservice(this);
	}
}

void FightRouterServiceLinkTo::on_authed(bool success)
{
	if (success)
	{
		logInfo(out_runtime, "me(GameService) connected to FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		this->set_authed(true);

		//sync your regist service
		svrApp.on_fightrouterservice_regist_result(this);
	}
	else
	{
		logInfo(out_runtime, "me(GameService) connect to FightRouter Service[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
	}

}

void FightRouterServiceLinkTo::on_disconnected()
{
	//need notify server, connection error
	if (this->is_authed())
	{
		logInfo(out_runtime, "me(GameService) disconnect from FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);
	}

	svrApp.on_disconnected_with_fightrouterservice(this);
}

void FightRouterServiceLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* FightRouterServiceLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}