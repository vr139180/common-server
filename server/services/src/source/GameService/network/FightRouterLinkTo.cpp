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

#include "network/FightRouterLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include <GameServiceApp.h>

FightRouterLinkTo::FightRouterLinkTo() :LinkToBase()
, node_(0)
{
	this->init_protocolhead();
}

FightRouterLinkTo::FightRouterLinkTo(ServiceNodeInfo* pnode) : LinkToBase()
, node_(pnode)
{
	this->init_protocolhead();
}

void FightRouterLinkTo::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GAME;
	s_head_.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER;
}

void FightRouterLinkTo::send_netprotocol(PRO::ERK_SERVICETYPE to, BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);

	SProtocolHead& head = pro->write_head();
	head.to_type_ = (S_INT_8)to;

	this->send_protocol(pro);
}

void FightRouterLinkTo::send_netprotocol(PRO::ERK_SERVICETYPE to, NetProtocol* pro)
{
	SProtocolHead& head = pro->write_head();
	head.to_type_ = (S_INT_8)to;

	this->send_protocol(pro);
}

void FightRouterLinkTo::reset(ServiceNodeInfo* pnode)
{
	node_.reset(pnode);
	force_close();
}

void FightRouterLinkTo::force_linkclose()
{
	force_close();
}

std::string FightRouterLinkTo::get_ext_bykey(const char* key)
{
	return node_->get_extparam_bykey(key);
}

void FightRouterLinkTo::connect()
{
	if (is_connected() || is_connecting())
		return;

	logInfo(out_runtime, "me(GameService) try to connect to FightRouter(iid:%ld ip:%s port:%d)",
		node_->iid, node_->ip.c_str(), node_->port);

	connect_to(node_->ip.c_str(), node_->port);
}

void FightRouterLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

	logInfo(out_runtime, "------me(GameService) cant connect to FightRouter(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&FightRouterLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void FightRouterLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

	logInfo(out_runtime, "++++++me(GameService) connected to FightRouter(iid:%ld ip:%s port:%d)++++++",
		node_->iid, node_->ip.c_str(), node_->port);

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&FightRouterLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void FightRouterLinkTo::on_connect_lost_netthread()
{
	SystemCommand<bool>* cmd = new SystemCommand<bool>(
		boost::bind(&FightRouterLinkTo::on_disconnected, this));
	svrApp.regist_syscmd(cmd);
}

void FightRouterLinkTo::on_recv_protocol_netthread( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	S_UINT_16 msgid = pro->get_msg();
	if (msgid == PRO::ERK_PROTYPE::SVR_SERVICEBINDSERVICE_ACK)
	{
		PRO::Svr_ServiceBindService_ack *ack = dynamic_cast<PRO::Svr_ServiceBindService_ack*>(pro->msg_);
		bool success = (ack->result() == 0);

		SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
			boost::bind(&FightRouterLinkTo::on_authed, this, boost::placeholders::_1), success);
		svrApp.regist_syscmd(cmd);
	}
}

void FightRouterLinkTo::on_connected(bool success)
{
	if (success)
	{
		//注册到home
		PRO::Svr_ServiceBindService_req *req = new PRO::Svr_ServiceBindService_req();

		req->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_GAME);
		req->set_myiid(EurekaClusterClient::instance().get_myiid());
		req->set_mytoken(EurekaClusterClient::instance().get_token());
		req->set_toiid(node_->iid);
		req->set_totoken(node_->token);

		this->send_netprotocol(PRO::ERK_SERVICE_FIGHTROUTER, req);
	}
	else
	{
		logError(out_runtime, "me(GameService) can't connect to FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);

		svrApp.on_disconnected_with_fightrouter(this);
	}
}

void FightRouterLinkTo::on_authed(bool success)
{
	if (success)
	{
		logInfo(out_runtime, "me(GameService) connected to FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);
		this->set_authed(true);

		//sync your regist service
		svrApp.on_fightrouter_regist_result(this);
	}
	else
	{
		logInfo(out_runtime, "me(GameService) connect to FightRouter Service[ip:%s port:%d] failed", node_->ip.c_str(), node_->port);
	}

}

void FightRouterLinkTo::on_disconnected()
{
	//need notify server, connection error
	if (this->is_authed())
	{
		logInfo(out_runtime, "me(GameService) disconnect from FightRouter Service[ip:%s port:%d]", node_->ip.c_str(), node_->port);
	}

	svrApp.on_disconnected_with_fightrouter(this);
}

void FightRouterLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

NetProtocol* FightRouterLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	NetProtocol* pro = new NetProtocol(get_protocolhead(), ntf);

	SProtocolHead& head = pro->write_head();
	head.router_balance_ = false;

	return pro;
}