#include "network/HomeServiceLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>
#include <gameLib/gatehome/ProtoTokenUtil.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

HomeServiceLinkTo::HomeServiceLinkTo() :LinkToBase()
, node_(0)
{
}

HomeServiceLinkTo::HomeServiceLinkTo(ServiceNodeInfo* pnode): LinkToBase()
,node_(pnode)
{
}

void HomeServiceLinkTo::reset(ServiceNodeInfo* pnode, S_INT_64 bindtoken)
{
	bind_token_ = bindtoken;
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

	logInfo(out_runtime, "me(GateService) try to connect to HomeService(iid:%ld ip:%s port:%d bindtoken:%lld)",
		node_->iid, node_->ip.c_str(), node_->port, bind_token_);

	connect_to(node_->ip.c_str(), node_->port);
}


void HomeServiceLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_runtime, "------me(GateService) cant connect to Home Service(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&HomeServiceLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void HomeServiceLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_runtime, "++++++me(GateService) connected to Home Service(iid:%ld ip:%s port:%d)++++++",
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
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&HomeServiceLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
	else if (proiid == PRO::ERK_PROTYPE::RES_PROXYUSERSLOT_REQ)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(
			&GamePlayerCtrl::on_res_proxyuserslot_req, &GamePlayerCtrl::instance(), boost::placeholders::_1, boost::placeholders::_2, this);

		NetCommandV *pcmd = new NetCommandV(p_msg.release(), fun);
		svrApp.regist_syscmd(pcmd);
	}
	else if (proiid == PRO::USER_PROTYPE::USER_ROLESELECT_ACK)
	{
		int slot = 0;
		if (ProtoUtil::get_slot_from_tokenx(pro, slot))
		{
			PlayerChannel* pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot);
			if (pchannel != 0)
			{
				NETCMD_FUN_MAP2 fun = boost::bind(&PlayerChannel::NetProcessMessage, pchannel,
					boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

				NetCommand *pcmd = new NetCommand(p_msg.release(), fun, (int)proiid);
				pchannel->regist_netcmd(pcmd);
			}
		}
	}
	else
	{
		int slot = 0;
		if (ProtoUtil::get_slot_from_tokenx(pro, slot))
		{
			GamePlayerCtrl::instance().route_msg_to_player(p_msg.release(), slot);
			if (proiid == PRO::USER_PROTYPE::USER_ROLELIST_ACK)
			{
				logDebug(out_runtime, "gate recv rolelist_ack message");
			}
			else
			{
				logDebug(out_runtime, "gate recv message:%d route to user", proiid);
			}
		}
	}
}

void HomeServiceLinkTo::on_connected( bool success)
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
		logError(out_runtime, "me(GateService) can't connect to HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		svrApp.on_disconnected_with_homeservice(this);
    }
}

void HomeServiceLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_runtime, "me(GateService) connected to HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		this->set_authed( true);

		//sync your regist service
		svrApp.on_homeservice_regist_result( this);
	}
    else
    {
		logInfo(out_runtime, "me(GateService) connect to HomeService[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }
}

void HomeServiceLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_runtime, "me(GateService) disconnect from HomeService[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

    this->force_close();
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

void HomeServiceLinkTo::bind_home_confirm()
{
	PRO::Svr_GateBindHome_confirm *confirm = new PRO::Svr_GateBindHome_confirm();
	confirm->set_gateiid(EurekaClusterClient::instance().get_myiid());
	confirm->set_homeiid(node_->iid);
	confirm->set_hometoken(node_->token);
	confirm->set_bindtoken(bind_token_);

	send_protocol(confirm);
}