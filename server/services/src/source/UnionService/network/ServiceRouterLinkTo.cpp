// Copyright 2021 common-server Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "network/ServiceRouterLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include "UnionServiceApp.h"

ServiceRouterLinkTo::ServiceRouterLinkTo() :LinkToBase()
, node_(0)
{
	this->init_protocolhead();
}

ServiceRouterLinkTo::ServiceRouterLinkTo(ServiceNodeInfo* pnode): LinkToBase()
,node_(pnode)
{
	this->init_protocolhead();
}

void ServiceRouterLinkTo::init_protocolhead()
{
	//����ͨ��Э��ͷ
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_UNION;
	s_head_.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER;
}

void ServiceRouterLinkTo::send_netprotocol(PRO::ERK_SERVICETYPE to, BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol( get_protocolhead(), msg);

	SProtocolHead& head = pro->write_head();
	head.to_type_ = (S_INT_8)to;

	this->send_protocol(pro);
}

void ServiceRouterLinkTo::send_netprotocol(PRO::ERK_SERVICETYPE to, NetProtocol* pro)
{
	SProtocolHead& head = pro->write_head();
	head.to_type_ = (S_INT_8)to;

	this->send_protocol(pro);
}

void ServiceRouterLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void ServiceRouterLinkTo::force_linkclose()
{
	force_close();
}

std::string ServiceRouterLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void ServiceRouterLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

	logInfo(out_runtime, "me(UnionService) try to connect to ServiceRouter(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void ServiceRouterLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_runtime, "------me(UnionService) cant connect to ServiceRouter(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&ServiceRouterLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void ServiceRouterLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_runtime, "++++++me(UnionService) connected to ServiceRouter(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&ServiceRouterLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void ServiceRouterLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>( 
		boost::bind( &ServiceRouterLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void ServiceRouterLinkTo::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);

	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro->msg_);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&ServiceRouterLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
}

void ServiceRouterLinkTo::on_connected( bool success)
{
    if( success)
    {
		//ע�ᵽhome
		PRO::Svr_ServiceBindService_req *req = new PRO::Svr_ServiceBindService_req();

		req->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_UNION);
		req->set_myiid(EurekaClusterClient::instance().get_myiid());
		req->set_mytoken(EurekaClusterClient::instance().get_token());
		req->set_toiid(node_->iid);
		req->set_totoken(node_->token);

		this->send_netprotocol(PRO::ERK_SERVICE_SVRROUTER, req);
    }
    else
    {
		logError(out_runtime, "me(UnionService) can't connect to ServiceRouter[ip:%s port:%d]", node_->ip.c_str(), node_->port);

		svrApp.on_disconnected_with_servicerouter(this);
    }
}

void ServiceRouterLinkTo::on_authed( bool success)
{
    if( success)
    {
		logInfo(out_runtime, "me(UnionService) connected to ServiceRouter[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		this->set_authed( true);

		//sync your regist service
		svrApp.on_servicerouter_regist_result( this);
	}
    else
    {
		logInfo(out_runtime, "me(UnionService) connect to ServiceRouter[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
    }

}

void ServiceRouterLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
		logInfo(out_runtime, "me(UnionService) disconnect from ServiceRouter[ip:%s port:%d]", node_->ip.c_str(), node_->port);
    }

	svrApp.on_disconnected_with_servicerouter(this);
}

void ServiceRouterLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

NetProtocol* ServiceRouterLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	NetProtocol* pro = new NetProtocol(get_protocolhead(), ntf);

	SProtocolHead& head = pro->write_head();
	head.to_type_ = (S_INT_8)PRO::ERK_SERVICE_SVRROUTER;

	return pro;
}