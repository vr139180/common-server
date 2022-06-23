#include "network/UnionClusterLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <GateServiceApp.h>

UnionClusterLinkTo::UnionClusterLinkTo():LinkToBase()
, node_(0)
{
}

void UnionClusterLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void UnionClusterLinkTo::force_linkclose()
{
	force_close();
}

std::string UnionClusterLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void UnionClusterLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo(out_boot, "me(GateService) try to connect to UnionService(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void UnionClusterLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_net, "------me(GateService) cant connect to Union Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&UnionClusterLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void UnionClusterLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_net, "++++++me(GateService) connected to Union Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&UnionClusterLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void UnionClusterLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>( 
		boost::bind( &UnionClusterLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void UnionClusterLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
}

void UnionClusterLinkTo::on_connected( bool success)
{
    if( success)
    {
		//ע�ᵽhome
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
        logError( out_boot, "me(GateService) can't connect to UnionService[iid:%ld ip:%s port:%d]", 
			node_->iid, node_->ip.c_str(), node_->port);

		svrApp.on_disconnected_with_unionservice(this);
    }
}

void UnionClusterLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_boot, "me(GateService) connected to UnionService[iid:%ld ip:%s port:%d]", 
			node_->iid, node_->ip.c_str(), node_->port);
        this->set_authed( true);

		//sync your regist service
		//svrApp.on_unionservice_regist_result(this);
    }
    else
    {
		logInfo(out_boot, "me(GateService) connect to UnionService[iid:%ld ip:%s port:%d]",
			node_->iid, node_->ip.c_str(), node_->port);
    }
}

void UnionClusterLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_boot, "me(GateService) disconnect from UnionService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

	svrApp.on_disconnected_with_unionservice(this);
}

void UnionClusterLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* UnionClusterLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}