#include "network/HomeServiceLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <GameServiceApp.h>

HomeServiceLinkTo::HomeServiceLinkTo() :LinkToBase()
, node_(0)
{
}

void HomeServiceLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void HomeServiceLinkTo::force_linkclose()
{
	force_close();
}

std::string HomeServiceLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void HomeServiceLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo(out_boot, "me(GameService) try to connect to HomeService(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void HomeServiceLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_net, "------me(GameService) cant connect to Home Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&HomeServiceLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void HomeServiceLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_net, "++++++me(GameService) connected to Home Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&HomeServiceLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void HomeServiceLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>(
		boost::bind( &HomeServiceLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void HomeServiceLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
}

void HomeServiceLinkTo::on_connected( bool success)
{
    if( success)
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
		logError(out_boot, "me(GameService) can't connect to HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		svrApp.on_disconnected_with_homeservice(this);
    }
}

void HomeServiceLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_boot, "me(GameService) connected to HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
        this->set_authed( true);

		//sync your regist service
		svrApp.on_homeservice_regist_result(this);
    }
    else
    {
		logInfo(out_boot, "me(GameService) connect to HomeService[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }
}

void HomeServiceLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_boot, "me(GameService) disconnect from HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);

    }

	svrApp.on_disconnected_with_homeservice(this);
}

void HomeServiceLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* HomeServiceLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}