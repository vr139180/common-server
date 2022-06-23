#include "network/RouterServiceLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

RouterServiceLinkTo::RouterServiceLinkTo() :LinkToBase()
, node_(0)
{
}

RouterServiceLinkTo::RouterServiceLinkTo(ServiceNodeInfo* pnode): LinkToBase()
,node_(pnode)
{

}

void RouterServiceLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void RouterServiceLinkTo::force_linkclose()
{
	force_close();
}

std::string RouterServiceLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void RouterServiceLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo(out_boot, "me(GateService) try to connect to RouterService(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void RouterServiceLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_net, "------me(GateService) cant connect to Router Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&RouterServiceLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void RouterServiceLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_net, "++++++me(GateService) connected to Router Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&RouterServiceLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void RouterServiceLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>( 
		boost::bind( &RouterServiceLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void RouterServiceLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&RouterServiceLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
	else if (proiid == PRO::CHAT_PROTYPE::CHAT_GLOBALMSG_NTF)
	{
		//全局聊天模块，嵌入gate
		GamePlayerCtrl::instance().chat_globalmsg_notify(p_msg.release());
	}
	else
	{
		int slot = 0;
		if (ProtoUtil::get_slot_from_tokenx(pro, slot))
		{
			GamePlayerCtrl::instance().route_msg_to_player(p_msg.release(), slot);
		}
	}
}

void RouterServiceLinkTo::on_connected( bool success)
{
    if( success)
    {
		//注册到home
		PRO::Svr_ServiceBindService_req *req = new PRO::Svr_ServiceBindService_req();
		req->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_GATE);
		req->set_myiid(EurekaClusterClient::instance().get_myiid());
		req->set_mytoken(EurekaClusterClient::instance().get_token());
		req->set_toiid(node_->iid);
		req->set_totoken(node_->token);

		this->send_protocol(req);
    }
    else
    {
		logError(out_boot, "me(GateService) can't connect to RouterService[ip:%s port:%d]", node_->ip.c_str(), node_->port);

		svrApp.on_disconnected_with_routerservice(this);
    }
}

void RouterServiceLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_boot, "me(GateService) connected to RouterService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		this->set_authed( true);

		//sync your regist service
		svrApp.on_routerservice_regist_result( this);
	}
    else
    {
		logInfo(out_boot, "me(GateService) connect to RouterService[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }

}

void RouterServiceLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_boot, "me(GateService) disconnect from RouterService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

	svrApp.on_disconnected_with_routerservice(this);
}

void RouterServiceLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* RouterServiceLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}