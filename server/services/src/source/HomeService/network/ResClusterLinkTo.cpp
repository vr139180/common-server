#include "network/ResClusterLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <HomeServiceApp.h>

ResClusterLinkTo::ResClusterLinkTo():LinkToBase()
, node_( 0)
{
}

ResClusterLinkTo::ResClusterLinkTo(ServiceNodeInfo *node) : LinkToBase()
, node_(node)
{
}

void ResClusterLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void ResClusterLinkTo::force_linkclose()
{
	force_close();
}

std::string ResClusterLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void ResClusterLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo( out_boot, "me(HomeService) try to connect to ResService(iid:%ld ip:%s port:%d)", 
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void ResClusterLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_net, "------me(GateService) cant connect to Res Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&ResClusterLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void ResClusterLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_net, "++++++me(GateService) connected to Res Service(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&ResClusterLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void ResClusterLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>( 
		boost::bind( &ResClusterLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void ResClusterLinkTo::on_recv_protocol_netthread(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::auto_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&ResClusterLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
	else if (proiid == PRO::ERK_PROTYPE::RES_PROXYUSERSLOT_REQ)
	{
		NETCMD_FUN_MAP fun = boost::bind(
			&HomeServiceApp::on_mth_userproxyslot_req, &svrApp, boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(p_msg.release(), fun);
		svrApp.regist_syscmd(pcmd);
	}
}

void ResClusterLinkTo::on_connected( bool success)
{
    if( success)
    {
		//ע�ᵽhome
		PRO::Svr_ServiceBindService_req *req = new PRO::Svr_ServiceBindService_req();
		req->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_HOME);
		req->set_myiid(EurekaClusterClient::instance().get_myiid());
		req->set_mytoken(EurekaClusterClient::instance().get_token());
		req->set_toiid(node_->iid);
		req->set_totoken(node_->token);

		this->send_protocol(req);
    }
    else
    {
        logError( out_boot, "me(HomeService) can't connect to ResService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		svrApp.on_disconnected_with_resservice(this);
    }
}

void ResClusterLinkTo::on_authed( bool success)
{
    if( success)
    {
        logInfo( out_boot, "me(HomeService) connected to ResService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
        this->set_authed( true);

		svrApp.on_resservice_regist_result(this);
	}
    else
    {
        logInfo( out_boot, "me(HomeService) connect to ResService[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }

}

void ResClusterLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
        logInfo( out_boot, "me(HomeService) disconnect from ResService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

	svrApp.on_disconnected_with_resservice(this);
}

void ResClusterLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

BasicProtocol* ResClusterLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	return ntf;
}
