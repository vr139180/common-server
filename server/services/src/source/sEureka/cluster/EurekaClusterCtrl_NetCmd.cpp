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

#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

void EurekaClusterCtrl::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF, &EurekaClusterCtrl::on_eurekaupdate_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_MASTERCHANGE_NTF, &EurekaClusterCtrl::on_masterchange_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESYNC_NTF, &EurekaClusterCtrl::on_servicesync_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESHUTDOWN_NTF, &EurekaClusterCtrl::on_serviceshutdown_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_EUREKALOST_NTF, &EurekaClusterCtrl::on_eurekalost_ntf, this);
}

void EurekaClusterCtrl::on_eurekaregist_req(NetProtocol* pro, bool& autorelease, void* session)
{
	EurekaSession* pes = reinterpret_cast<EurekaSession*>(session);
	Erk_EurekaRegist_req* req = reinterpret_cast<Erk_EurekaRegist_req*>(pro->msg_);

	//只有master能处理
	if (!is_master() || !is_boosted())
	{
		SProtocolHead head;
		head.router_balance_ = false;
		head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
		head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;

		Erk_EurekaRegist_ack* ack = new Erk_EurekaRegist_ack();
		ack->set_result(1);

		pes->send_to_service(head, ack);

		return;
	}

	EurekaNodeInfo enode;
	enode.iid = this->make_next_eurekaiid();
	enode.token = (S_INT_64)OSSystem::mOS->GetTimestamp();
	enode.ip = req->ip().c_str();
	enode.port = req->port();
	enode.ismaster = false;

	//建立关联
	EurekaLinkFrom* pfrom = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(pes);
		pfrom = eureka_links_from_.ask_free_link();

		pfrom->set_linkbase_info( enode.iid, enode.token);

		pes->auth();
		pfrom->set_session(pes);
		pes->set_netlinkbase(pfrom);

		//设置当前gatelinke
		eureka_links_from_.regist_onlinelink(pfrom);

		pfrom->registinfo_tolog(true);
		pfrom->set_node(enode);
	}

	//管理节点
	master_regist_one_slaver( enode);

	//发送回复
	Erk_EurekaRegist_ack* ack = new Erk_EurekaRegist_ack();
	ack->set_result(0);
	ack->set_myiid(enode.iid);
	ack->set_mytoken(enode.token);
	ack->set_eureka_seed(last_eureka_iid_);
	ack->set_service_seed(svrApp.get_servicectrl()->get_serviceiid_seed());

	pfrom->send_to_eureka(ack);

	//同步全量信息
	master_sync_all_to_slaver(pfrom);
	master_notify_change_to_slaver(pfrom);
}

void EurekaClusterCtrl::on_eurekabind_req(NetProtocol* pro, bool& autorelease, void* session)
{
	EurekaSession* pes = reinterpret_cast<EurekaSession*>(session);
	Erk_EurekaBind_req* req = reinterpret_cast<Erk_EurekaBind_req*>(pro->msg_);

	EurekaNodeInfo *pnode = get_eureka_node(req->iid());
	if ( pnode == 0 || (!is_boosted()))
	{
		SProtocolHead head;
		head.router_balance_ = false;
		head.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;
		head.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_EUREKA;

		Erk_EurekaBind_ack* ack = new Erk_EurekaBind_ack();
		ack->set_result(2);

		pes->send_to_service(head, ack);
		return;
	}
	
	//建立关联
	EurekaLinkFrom* pfrom = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(pes);
		pfrom = eureka_links_from_.ask_free_link();

		pfrom->set_linkbase_info(req->iid(), req->token());

		pes->auth();
		pfrom->set_session(pes);
		pes->set_netlinkbase(pfrom);

		//设置当前gatelinke
		eureka_links_from_.regist_onlinelink(pfrom);

		pfrom->registinfo_tolog(true);

		pfrom->set_node(*pnode);
	}

	Erk_EurekaBind_ack *ack = new Erk_EurekaBind_ack();
	ack->set_result(0);
	pfrom->send_to_eureka(ack);

	//断线之后再次绑定到master
	if (is_master() && is_boosted())
	{
		//同步全量信息
		master_sync_all_to_slaver(pfrom);
	}

	//断线处理
	someone_eurekanode_authed(req->iid());
}

void EurekaClusterCtrl::on_eurekaregist_ack(NetProtocol* pro, bool& autorelease, EurekaLinkTo* pLinkto)
{
	Erk_EurekaRegist_ack *ack = reinterpret_cast<Erk_EurekaRegist_ack*>(pro->msg_);

	//设置认证状态
	pLinkto->on_authed(ack->result() == 0);

	//认证成功
	if (pLinkto->is_ready())
	{
		this->myself_.iid = ack->myiid();
		this->myself_.token = ack->mytoken();
		this->last_eureka_iid_ = ack->eureka_seed();
		svrApp.get_servicectrl()->set_serviceiid_seed(ack->service_seed());

		//收到eureka_update，再设置启动标志
		return;
	}

	//连接的不是master，启动失败
	if (ack->result() == 1)
	{
		svrApp.quit_app();
		return;
	}

	//断开连接,放入重连队列
	this->on_disconnected_with_linkto(pLinkto);
}

void EurekaClusterCtrl::on_eurekabind_ack(NetProtocol* pro, bool& autorelease, EurekaLinkTo* pLinkto)
{
	Erk_EurekaBind_ack *ack = reinterpret_cast<Erk_EurekaBind_ack*>(pro->msg_);

	//设置认证状态
	pLinkto->on_authed(ack->result() == 0);

	//认证成功
	if (pLinkto->is_ready())
	{
		//断线处理
		someone_eurekanode_authed(pLinkto->get_iid());

		return;
	}

	//断开连接,放入重连队列
	this->on_disconnected_with_linkto(pLinkto);
}

void EurekaClusterCtrl::on_eurekaupdate_ntf(NetProtocol* pro, bool& autorelease)
{
	if (is_master())
		return;

	Erk_EurekaUpdate_ntf* ntf = reinterpret_cast<Erk_EurekaUpdate_ntf*>(pro->msg_);

	if (!is_boosted())
	{
		//标记启动成功
		this->mark_boosted();
		svrApp.on_notify_boosted();
	}

	//update somthing
	this->last_eureka_iid_ = ntf->eureka_seed();
	svrApp.get_servicectrl()->set_serviceiid_seed(ntf->service_seed());
	this->eureka_master_iid_ = ntf->masteriid();

	slaver_sync_eurekanodes(ntf);
}

void EurekaClusterCtrl::on_masterchange_ntf(NetProtocol* pro, bool& autorelease)
{

}

void EurekaClusterCtrl::on_servicesync_ntf(NetProtocol* pro, bool& autorelease)
{
	svrApp.get_servicectrl()->on_mth_servicesync_ntf(pro, autorelease);
}

void EurekaClusterCtrl::on_serviceshutdown_ntf(NetProtocol* pro, bool& autorelease)
{
	svrApp.get_servicectrl()->on_mth_serviceshutdown_ntf(pro, autorelease);
}

void  EurekaClusterCtrl::on_eurekalost_ntf(NetProtocol* pro, bool& autorelease)
{
	Erk_EurekaLost_ntf* ntf = reinterpret_cast<Erk_EurekaLost_ntf*>(pro->msg_);

	tellme_eurekanode_lost(ntf->from_eureka(), ntf->lost_eureka());
}