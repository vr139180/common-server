#include "network/GameServiceLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <GateServiceApp.h>

GameServiceLinkTo::GameServiceLinkTo() :LinkToBase()
, node_(0)
{
}

void GameServiceLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void GameServiceLinkTo::force_linkclose()
{
	force_close();
}

std::string GameServiceLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void GameServiceLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo(out_boot, "me(GateService) try to connect to GameService(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void GameServiceLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_net, "------me(GateService) cant connect to Game Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&GameServiceLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void GameServiceLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_net, "++++++me(GateService) connected to Game Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&GameServiceLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void GameServiceLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>( 
		boost::bind( &GameServiceLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void GameServiceLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
}

void GameServiceLinkTo::on_connected( bool success)
{
    if( success)
    {
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
		logError(out_boot, "me(GateService) can't connect to GameService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		svrApp.on_disconnected_with_gameservice(this);
    }
}

void GameServiceLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_boot, "me(GateService) connected to GameService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
        this->set_authed( true);

		//sync your regist service
		//svrApp.on_gameservice_regist_result(this);
    }
    else
    {
		logInfo(out_boot, "me(GateService) connect to GameService[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }
}

void GameServiceLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_boot, "me(GateService) disconnect from GameService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

	svrApp.on_disconnected_with_gameservice(this);
}

void GameServiceLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* GameServiceLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}