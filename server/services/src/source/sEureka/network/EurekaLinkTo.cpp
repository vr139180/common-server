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

#include "network/EurekaLinkTo.h"

#include <cmsLib/system/CommandBase.h>

#include <gameLib/LogExt.h>

#include "sEurekaApp.h"

EurekaLinkTo::EurekaLinkTo(EurekaNodeInfo p):LinkToBase()
, fail_num_(0)
{
	node_ = p;
	this->init_protocolhead();
}

EurekaLinkTo::~EurekaLinkTo()
{
	reset(EurekaNodeInfo());
}

void EurekaLinkTo::init_protocolhead()
{
	//设置通用协议头
	s_head_.router_balance_ = false;
	s_head_.hashkey_ = 0;
	s_head_.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
	s_head_.to_broadcast_ = false;
	s_head_.unpack_protocol_ = true;
}

void EurekaLinkTo::send_to_eureka(BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(get_protocolhead(), msg);
	
	LinkToBase::send_protocol(pro);
}

void EurekaLinkTo::reset(EurekaNodeInfo pinfo)
{
	fail_num_ = 0;
	node_ = pinfo;

	force_close();
}

void EurekaLinkTo::force_linkclose()
{
	force_close();
}

void EurekaLinkTo::connect()
{
    if( is_connected() || is_connecting())
        return;

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_runtime, "++++++me(sEureka) try to connect to sEureka(iid:%ld ip:%s port:%d)++++++",
		node_.iid, node_.ip.c_str(), node_.port);
#endif

	connect_to(node_.ip.c_str(), node_.port);
}

void EurekaLinkTo::on_cant_connectedto()
{
	LinkToBase::on_cant_connectedto();

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_runtime, "------me(sEureka) cant connect to sEureka(iid:%ld ip:%s port:%d)------",
		node_->iid, node_->ip.c_str(), node_->port);
#endif

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaLinkTo::on_connected, this, boost::placeholders::_1), false);
	svrApp.regist_syscmd(cmd);
}

void EurekaLinkTo::on_connectedto_done()
{
	LinkToBase::on_connectedto_done();

#ifdef EUREKA_DEBUGINFO_ENABLE
	logInfo(out_runtime, "++++++me(sEureka) connected to sEureka(iid:%ld ip:%s port:%d)++++++",
		node_.iid, node_.ip.c_str(), node_.port);
#endif

	SystemCommand2<bool>* cmd = new SystemCommand2<bool>(
		boost::bind(&EurekaLinkTo::on_connected, this, boost::placeholders::_1), true);
	svrApp.regist_syscmd(cmd);
}

void EurekaLinkTo::on_connect_lost_netthread()
{
    SystemCommand<bool>* cmd =new SystemCommand<bool>(
		boost::bind( &EurekaLinkTo::on_disconnected, this));
    svrApp.regist_syscmd( cmd);
}

void EurekaLinkTo::on_recv_protocol_netthread( NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	
	if (msgid == ERK_PROTYPE::ERK_EUREKAREGIST_ACK)
	{
		NETCMD_FUN_MAP3 fun = boost::bind(&EurekaClusterCtrl::on_eurekaregist_ack, svrApp.get_eurekactrl(),
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
	}
	else if (msgid == ERK_PROTYPE::ERK_EUREKABIND_ACK)
    {
		NETCMD_FUN_MAP3 fun = boost::bind(&EurekaClusterCtrl::on_eurekabind_ack, svrApp.get_eurekactrl(),
			boost::placeholders::_1, boost::placeholders::_2, this);
		NetCommandV *pcmd = new NetCommandV(pro, fun);

		svrApp.regist_syscmd(pcmd);
    }
    else
    {
		NETCMD_FUN_MAP fun = boost::bind(
			&EurekaClusterCtrl::NetProcessMessage, svrApp.get_eurekactrl(), boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand( pro, fun);
		svrApp.regist_syscmd(pcmd);
    }
}

void EurekaLinkTo::on_connected( bool success)
{
    if( success)
    {
		EurekaNodeInfo& myself = svrApp.get_eurekactrl()->get_myself();

		//发送绑定请求
		bool regist = false;
		if (svrApp.get_eurekactrl()->check_node_is_master(node_.iid))
		{
			regist = true;
			//已经注册成功了，丢失连接只做bind
			if (svrApp.get_eurekactrl()->is_boosted())
				regist = false;
		}

		if (regist)
		{
			PRO::Erk_EurekaRegist_req* req = new PRO::Erk_EurekaRegist_req();
			req->set_ip(myself.ip);
			req->set_port(myself.port);

			this->send_to_eureka(req);
		}
		else
		{
			PRO::Erk_EurekaBind_req* req = new PRO::Erk_EurekaBind_req();
			req->set_iid(myself.iid);
			req->set_token(myself.token);

			this->send_to_eureka(req);

			logInfo(out_runtime, "------ send bind request from:%d to:%d", myself.iid, node_.iid);
		}
    }
    else
    {
		++fail_num_;
		logError(out_runtime, "me(Eureka) can't connect to Eureka[ip:%s port:%d]", node_.ip.c_str(), node_.port);

		svrApp.get_eurekactrl()->on_disconnected_with_linkto(this);
    }
}

void EurekaLinkTo::on_authed( bool success)
{
    if( success)
    {
        logInfo(out_runtime, ">>>>>> me(sEureka) auth to sEureka[iid:%ld ip:%s port:%d]!!!!",
			node_.iid, node_.ip.c_str(), node_.port);
        this->set_authed( true);
		
		svrApp.get_eurekactrl()->on_linkto_regist_result(this);
	}
    else
    {
		logInfo(out_runtime, "me(sEureka) connect to sEureka[iid:%ld ip:%s port:%d] failed", node_.iid, node_.ip.c_str(), node_.port);
    }
}

void EurekaLinkTo::on_disconnected()
{
    //need notify server, connection error
    if( this->is_authed())
    {
        logInfo(out_runtime, "<<<<<< me(sEureka)(authed) lost connection to sEureka[iid:%ld ip:%s port:%d]",
			node_.iid, node_.ip.c_str(), node_.port);
    }

	svrApp.get_eurekactrl()->on_disconnected_with_linkto(this);
}

void EurekaLinkTo::heart_beat()
{
	LinkToBase::heart_beat();
}

NetProtocol* EurekaLinkTo::get_livekeep_msg()
{
	PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();
	NetProtocol* msg = new NetProtocol(get_protocolhead(), ntf);

	return msg;
}
