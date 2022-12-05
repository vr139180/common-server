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

#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>
#include <gameLib/eureka/GateNodeInfo.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE

void ServiceRegisterCtrl::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ, &ServiceRegisterCtrl::on_mth_servicesubscribe_req, this);
	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_REQ, &ServiceRegisterCtrl::on_mth_routersubscribe_req, this);
	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERONLINE_REQ, &ServiceRegisterCtrl::on_mth_routeronline_req, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESYNC_NTF, &ServiceRegisterCtrl::on_mth_servicesync_ntf, this);
	REGISTERMSG(ERK_PROTYPE::SVR_GATESLOTUPDATE_NTF, &ServiceRegisterCtrl::on_mth_gateupdate_ntf, this);
}

void ServiceRegisterCtrl::on_mth_serviceregist_req(NetProtocol* pro, bool& autorelease, void* session)
{
	Erk_ServiceRegist_req* req = dynamic_cast<Erk_ServiceRegist_req*>(pro->msg_);
	EurekaSession* psession = reinterpret_cast<EurekaSession*>(session);

	bool bmaster = svrApp.get_eurekactrl()->is_master();

	//必须是master节点才能接收注册
	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE || !bmaster)
	{
		Erk_ServiceRegist_ack* ack = new Erk_ServiceRegist_ack();
		if( !bmaster)
			ack->set_result(2);
		else
			ack->set_result(1);
		
		SProtocolHead head = pro->head_;
		head.from_type_ = head.to_type_;
		head.to_type_ = pro->head_.from_type_;

		psession->send_to_service( head, ack);
		return;
	}

	//生成服务iid,全局唯一
	S_INT_64 sid = make_next_serviceiid();

	ServiceNodeInfo pnode;
	pnode.iid = sid;
	pnode.type = ctype;
	pnode.token = (S_INT_64)OSSystem::mOS->GetTimestamp();
	pnode.ip = req->ip().c_str();
	pnode.port = req->port();
	pnode.isrouter = req->isrouter();
	//router服务在线状态需要确认，其他默认为online
	if (req->isrouter())
		pnode.isonline = false;
	else
		pnode.isonline = true;

	if (req->exts_size() > 0)
	{
		const google::protobuf::Map<std::string,std::string>& kvs = req->exts();
		for (google::protobuf::Map<std::string, std::string>::const_iterator iter = kvs.cbegin(); 
			iter != kvs.cend(); ++iter)
		{
			pnode.extparams[iter->first] = iter->second;
		}
	}

	ServiceLinkFrom* pLink = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(psession);

		pLink = service_mth_links_.ask_free_link();

		//关联node
		pLink->set_node(&pnode);
		pLink->set_exts(pnode.extparams);

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//登记到服务连接映射表
		service_mth_links_.regist_onlinelink(pLink);
	}

	pLink->registinfo_tolog(true);

	regist_one_service(pnode);

	if (pnode.type == NETSERVICE_TYPE::ERK_SERVICE_GATE)
		regist_gate_to_redis(pnode);

	Erk_ServiceRegist_ack *ack = new Erk_ServiceRegist_ack();
	ack->set_result(0);
	ack->set_iid(sid);
	ack->set_token( pnode.token);
	ack->set_eurekaiid(svrApp.get_eurekactrl()->get_myself().iid);
	ack->set_eurekatoken(svrApp.get_eurekactrl()->get_myself().token);

	pLink->send_to_service(ack);

	//同步增量服务
	Erk_ServiceSync_ntf* ntf = new Erk_ServiceSync_ntf();
	std::unique_ptr<Erk_ServiceSync_ntf> ptr(ntf);

	ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
	ntf->set_fullsvrs(false);
	ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
	ntf->set_service_seed(this->get_serviceiid_seed());

	PRO::ServerNode *pnew = ntf->add_newsvrs();
	pnode.copy_to(pnew);

	svrApp.get_eurekactrl()->broadcast_to_eurekas(ntf);

	notify_service_online(sid);
}

void ServiceRegisterCtrl::on_mth_servicebind_req(NetProtocol* pro, bool& autorelease, void* session)
{
	Erk_ServiceBind_req* req = reinterpret_cast<Erk_ServiceBind_req*>(pro->msg_);
	EurekaSession* psession = reinterpret_cast<EurekaSession*>(session);

	std::string siid = std::to_string(req->iid());

	Erk_ServiceBind_ack* ack = new Erk_ServiceBind_ack();
	ack->set_result(0);

	ServiceNodeInfo* pnode = find_servicenode_byiid(req->iid());
	bool bmaster = svrApp.get_eurekactrl()->is_master();

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE || pnode == 0 || bmaster)
	{
		ack->set_result(1);

		SProtocolHead head = pro->head_;
		head.from_type_ = head.to_type_;
		head.to_type_ = pro->head_.from_type_;

		psession->send_to_service( head, ack);

		return;
	}

	ServiceLinkFrom* pLink = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//服务已经在线
		if (service_mth_links_.get_servicelink_byiid(req->iid()) != 0)
		{
			ack->set_result(1);
			
			SProtocolHead head = pro->head_;
			head.from_type_ = head.to_type_;
			head.to_type_ = pro->head_.from_type_;
			psession->send_to_service(head, ack);
			return;
		}

		svrApp.remove_waitsession_no_mutex(psession);

		pLink = service_mth_links_.ask_free_link();

		//关联node
		pLink->set_node( pnode);

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		service_mth_links_.regist_onlinelink(pLink);
	}

	pLink->registinfo_tolog(true);
	pLink->send_to_service(ack);
}

void ServiceRegisterCtrl::on_mth_servicesubscribe_req(NetProtocol* pro, bool& autorelease)
{
	bool bmaster = svrApp.get_eurekactrl()->is_master();
	if (!bmaster)
	{
		logError(out_runtime, "eureka[%d] slaver node recv a Erk_ServiceSubscribe_req request....", svrApp.get_eurekactrl()->get_myself().iid);
		return;
	}

	Erk_ServiceSubscribe_req* req = dynamic_cast<Erk_ServiceSubscribe_req*>(pro->msg_);

	//-------------------------协议同步-----------------------
	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(req->myiid());
	ServiceNodeInfo* pNode = find_servicenode_byiid(req->myiid());
	if (plink == 0 || pNode == 0)
		return;

	bool bdiff = false;
	for (int ii = 0; ii < req->svr_type_size(); ++ii)
	{
		const Erk_ServiceSubscribe_req_svrinfo& sinfo = req->svr_type(ii);
		NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)sinfo.svr_type();
		if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
			continue;

		//保存一个注册
		if (pNode->add_subscribe(ctype))
		{
			bdiff = true;

			//建立关联信息
			add_service_to_subscribe(ctype, req->myiid());
		}

		std::set<S_INT_64> offline;
		for (int kk = 0; kk < sinfo.exits_size(); ++kk)
		{
			offline.insert(sinfo.exits(kk));
		}

		//请求同类型订阅,需要过滤同ip+port的服务,防止连接自己
		bool checkMyIpPort = (((NETSERVICE_TYPE)req->mysvrtype()) == ctype);

		std::list<const ServiceNodeInfo*> onlines;
		const std::list<ServiceNodeInfo*>& pnodes = get_service_node_oftype(ctype);
		for (std::list<ServiceNodeInfo*>::const_iterator iter = pnodes.begin(); iter != pnodes.end(); ++iter)
		{
			//检查服务是否依然在线
			const ServiceNodeInfo* pinfo = (*iter);
			std::set<S_INT_64>::iterator fiter = offline.find(pinfo->iid);
			if (fiter == offline.end())
			{
				onlines.push_back(pinfo);
			}
			else
				offline.erase(fiter);
		}

		Erk_ServiceSubscribe_ntf* ack = new Erk_ServiceSubscribe_ntf();
		ack->set_myiid(req->myiid());
		ack->set_svr_type(ctype);

		for (std::set<S_INT_64>::iterator iter = offline.begin(); iter != offline.end(); ++iter)
		{
			ack->add_offsvrs((*iter));
		}
		for (std::list<const ServiceNodeInfo*>::iterator iter = onlines.begin(); iter != onlines.end(); ++iter)
		{
			const ServiceNodeInfo* pinfo = (*iter);

			PRO::ServerNode* pnod = ack->add_newsvrs();
			pinfo->copy_to(pnod);
		}

		plink->send_to_service(ack);
	}

	//有变化，需要eureka节点间同步
	if (bdiff)
	{
		Erk_ServiceSync_ntf* ntf = new Erk_ServiceSync_ntf();
		std::unique_ptr<Erk_ServiceSync_ntf> ptr(ntf);

		ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
		ntf->set_fullsvrs(false);
		ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
		ntf->set_service_seed(this->get_serviceiid_seed());

		PRO::ServerNode *pnew = ntf->add_newsvrs();
		pNode->copy_to(pnew);

		svrApp.get_eurekactrl()->broadcast_to_eurekas(ntf);
	}
}

void ServiceRegisterCtrl::on_mth_servicesync_ntf(NetProtocol* pro, bool& autorelease)
{
	bool bmaster = svrApp.get_eurekactrl()->is_master();
	if (bmaster)
		return;

	Erk_ServiceSync_ntf* ntf = dynamic_cast<Erk_ServiceSync_ntf*>(pro->msg_);

	svrApp.get_eurekactrl()->sync_slaver_eurekaseed(ntf->eureka_seed());
	this->set_serviceiid_seed(ntf->service_seed());

	if (ntf->fullsvrs())
	{
		//全量更新
		router_of_subscribe_.clear();
		service_of_subscribe_.clear();
		servie_of_type_.clear();
		all_service_nodes_.clear();

		for (int ii = 0; ii < ntf->newsvrs_size(); ++ii)
		{
			const PRO::ServerNode& pnod = ntf->newsvrs(ii);

			ServiceNodeInfo ninfo;
			ninfo.copy_from( &pnod);

			ServiceNodeInfo* psvr = regist_one_service(ninfo);

			for (std::list<NETSERVICE_TYPE>::iterator iter = psvr->subscribes_.begin(); iter != psvr->subscribes_.end(); ++iter)
			{
				NETSERVICE_TYPE ctype = (*iter);
				add_service_to_subscribe(ctype, ninfo.iid);
			}

			for (std::list<NETSERVICE_TYPE>::iterator iter = psvr->routers_.begin(); iter != psvr->routers_.end(); ++iter)
			{
				NETSERVICE_TYPE ctype = (*iter);
				add_service_to_router(ctype, ninfo.iid);
			}
		}
	}
	else
	{
		//增量更新
		for (int ii = 0; ii < ntf->newsvrs_size(); ++ii)
		{
			const PRO::ServerNode& pnod = ntf->newsvrs(ii);

			ServiceNodeInfo ninfo;
			ninfo.copy_from( &pnod);

			ServiceNodeInfo* psvr = regist_one_service(ninfo, true);

			for (std::list<NETSERVICE_TYPE>::iterator iter = psvr->subscribes_.begin(); iter != psvr->subscribes_.end(); ++iter)
			{
				NETSERVICE_TYPE ctype = (*iter);
				add_service_to_subscribe(ctype, ninfo.iid);
			}

			for (std::list<NETSERVICE_TYPE>::iterator iter = psvr->routers_.begin(); iter != psvr->routers_.end(); ++iter)
			{
				NETSERVICE_TYPE ctype = (*iter);
				add_service_to_router(ctype, ninfo.iid);
			}
		}

		//删除失效的
		for (int ii = 0; ii < ntf->offline_size(); ++ii)
		{
			S_INT_64 iid = ntf->offline(ii);
			offline_one_service(iid);
		}
	}
}

void ServiceRegisterCtrl::on_mth_routersubscribe_req(NetProtocol* pro, bool& autorelease)
{
	bool bmaster = svrApp.get_eurekactrl()->is_master();
	if (!bmaster)
	{
		logError(out_runtime, "eureka[%d] slaver node recv a Erk_RouterSubscribe_req request....", svrApp.get_eurekactrl()->get_myself().iid);
		return;
	}

	Erk_RouterSubscribe_req* req = dynamic_cast<Erk_RouterSubscribe_req*>(pro->msg_);

	//-------------------------协议同步-----------------------
	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(req->myiid());
	ServiceNodeInfo* pNode = find_servicenode_byiid(req->myiid());
	if (plink == 0 || pNode == 0 || !pNode->isrouter)
		return;

	bool bdiff = false;
	for (int ii = 0; ii < req->svr_types_size(); ++ii)
	{
		NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_types(ii);
		if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
			continue;

		//保存一个注册
		if (pNode->add_router(ctype))
		{
			bdiff = true;

			add_service_to_router(ctype, req->myiid());
		}

		if (pNode->isonline)
		{
			//订阅相关的router服务
			//一次订阅所有
			for (std::list<NETSERVICE_TYPE>::iterator iter = pNode->routers_.begin(); iter != pNode->routers_.end(); ++iter)
			{
				NETSERVICE_TYPE ctype = (*iter);

				Erk_RouterSubscribe_ntf* ack = new Erk_RouterSubscribe_ntf();
				ack->set_myiid(req->myiid());
				ack->set_svr_type(ctype);

				const std::list<ServiceNodeInfo*>& pnodes = get_service_node_oftype(ctype);
				for (std::list<ServiceNodeInfo*>::const_iterator iter = pnodes.begin(); iter != pnodes.end(); ++iter)
				{
					const ServiceNodeInfo* pinfo = (*iter);
					ack->add_svriids((S_INT_64)pinfo->iid);
				}

				plink->send_to_service(ack);
			}
		}
	}

	//有变化，需要eureka节点间同步
	if (bdiff)
	{
		Erk_ServiceSync_ntf* ntf = new Erk_ServiceSync_ntf();
		std::unique_ptr<Erk_ServiceSync_ntf> ptr(ntf);

		ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
		ntf->set_fullsvrs(false);
		ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
		ntf->set_service_seed(this->get_serviceiid_seed());

		PRO::ServerNode *pnew = ntf->add_newsvrs();
		pNode->copy_to(pnew);

		svrApp.get_eurekactrl()->broadcast_to_eurekas(ntf);
	}
}

void ServiceRegisterCtrl::on_mth_routeronline_req(NetProtocol* pro, bool& autorelease)
{
	bool bmaster = svrApp.get_eurekactrl()->is_master();
	if (!bmaster)
	{
		logError(out_runtime, "eureka[%d] slaver node recv a Erk_RouterOnline_req request....", svrApp.get_eurekactrl()->get_myself().iid);
		return;
	}

	Erk_RouterOnline_req* req = dynamic_cast<Erk_RouterOnline_req*>(pro->msg_);

	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(req->myiid());
	ServiceNodeInfo* pNode = find_servicenode_byiid(req->myiid());
	if (plink == 0 || pNode == 0)
		return;

	pNode->isonline = true;

	//online之后，获取改router节点订阅的router balance相关的信息
	//一次通过所有的订阅
	for (std::list<NETSERVICE_TYPE>::iterator iter = pNode->routers_.begin(); iter != pNode->routers_.end(); ++iter)
	{
		NETSERVICE_TYPE ctype = (*iter);

		Erk_RouterSubscribe_ntf* ack = new Erk_RouterSubscribe_ntf();
		ack->set_myiid(req->myiid());
		ack->set_svr_type(ctype);

		const std::list<ServiceNodeInfo*>& pnodes = get_service_node_oftype(ctype);
		for (std::list<ServiceNodeInfo*>::const_iterator iter = pnodes.begin(); iter != pnodes.end(); ++iter)
		{
			const ServiceNodeInfo* pinfo = (*iter);
			ack->add_svriids((S_INT_64)pinfo->iid);
		}

		plink->send_to_service(ack);
	}

	//向订阅该类服务的发送 online通知
	{
		NETSERVICE_TYPE ctype = pNode->type;
		std::list<S_INT_64>& subs = get_subscribes_oftype(ctype);
		for (std::list<S_INT_64>::iterator iter = subs.begin(); iter != subs.end(); ++iter)
		{
			ServiceLinkFrom* psvr = service_mth_links_.get_servicelink_byiid((*iter));
			if (psvr == 0)
				continue;

			Svr_RouterOnline_ntf* ntf = new Svr_RouterOnline_ntf();
			ntf->set_routeriid(pNode->iid);
			ntf->set_routertype(pNode->type);

			psvr->send_to_service(ntf);
		}
	}

	//sync to all slaver nodes
	Erk_ServiceSync_ntf* ntf = new Erk_ServiceSync_ntf();
	std::unique_ptr<Erk_ServiceSync_ntf> ptr(ntf);

	ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
	ntf->set_fullsvrs(false);
	ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
	ntf->set_service_seed(this->get_serviceiid_seed());

	PRO::ServerNode *pnew = ntf->add_newsvrs();
	pNode->copy_to(pnew);

	svrApp.get_eurekactrl()->broadcast_to_eurekas(ntf);
}

void ServiceRegisterCtrl::on_mth_gateupdate_ntf(NetProtocol* pro, bool& autorelease)
{
	bool bmaster = svrApp.get_eurekactrl()->is_master();
	if (!bmaster)
		return;

	PRO::Svr_GateSlotUpdate_ntf* ntf = dynamic_cast<PRO::Svr_GateSlotUpdate_ntf*>(pro->msg_);
	this->update_gate_to_redis(ntf->iid(), ntf->frees());
}

void ServiceRegisterCtrl::regist_gate_to_redis(ServiceNodeInfo& info)
{
	RedisClient* rdv = svrApp.get_redisclient();
	GateNodeInfo ginfo(info);

	std::string key = rdv->build_rediskey(rdkey::global::GATE_NODE, ginfo.iid);
	rdv->add(key.c_str(), ginfo, GATENODE_TIMEOUT);

	if (ginfo.socket_type == GATE_LINKTYPE_TCP)
		key = rdkey::global::GATE_TCP_LIST;
	else
		key = rdkey::global::GATE_WS_LIST;

	rdv->add_zset(key.c_str(), std::to_string(ginfo.iid).c_str(), GATE_PLAYER_MAX);
}

void ServiceRegisterCtrl::unregist_gate_from_redis(ServiceNodeInfo& info)
{
	RedisClient* rdv = svrApp.get_redisclient();
	GateNodeInfo ginfo(info);

	std::string key = rdv->build_rediskey(rdkey::global::GATE_NODE, ginfo.iid);
	rdv->del(key.c_str());

	if (ginfo.socket_type == GATE_LINKTYPE_TCP)
		key = rdkey::global::GATE_TCP_LIST;
	else
		key = rdkey::global::GATE_WS_LIST;
	rdv->del_zsetmember(key.c_str(), std::to_string(ginfo.iid).c_str());
}

void ServiceRegisterCtrl::update_gate_to_redis(S_INT_64 sid, S_INT_32 freeslot)
{
	ServiceNodeInfo* pnod = find_servicenode_byiid(sid);
	if (pnod == 0)
		return;
	GateNodeInfo ginfo( *pnod);

	logDebug(out_runtime, "gate:%lld update free slot:%d to redis, type:%d", sid, freeslot, ginfo.socket_type);

	RedisClient* rdv = svrApp.get_redisclient();

	std::string lkey = "";
	if (ginfo.socket_type == GATE_LINKTYPE_TCP)
		lkey = rdkey::global::GATE_TCP_LIST;
	else
		lkey = rdkey::global::GATE_WS_LIST;

	std::string key = rdv->build_rediskey(rdkey::global::GATE_NODE, sid);
	if (rdv->pexpire(key.c_str(), GATENODE_TIMEOUT))
		rdv->add_zset(lkey.c_str(), std::to_string(sid).c_str(), (S_INT_64)freeslot, UpdateType::EXIST);
	else
		rdv->del_zsetmember(lkey.c_str(), std::to_string(sid).c_str());
}