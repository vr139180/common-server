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

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE

void ServiceRegisterCtrl::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_REQ, &ServiceRegisterCtrl::on_mth_servicesubscribe_req, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_NTF, &ServiceRegisterCtrl::on_mth_servicesubscribe_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESYNC_NTF, &ServiceRegisterCtrl::on_mth_servicesync_ntf, this);

	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_REQ, &ServiceRegisterCtrl::on_mth_routersubscribe_req, this);
	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_NTF, &ServiceRegisterCtrl::on_mth_routersubscribe_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERONLINE_REQ, &ServiceRegisterCtrl::on_mth_routeronline_req, this);
	REGISTERMSG(ERK_PROTYPE::SVR_ROUTERONLINE_NTF, &ServiceRegisterCtrl::on_mth_routeronline_ntf, this);

	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESHUTDOWN_NTF, &ServiceRegisterCtrl::on_mth_serviceshutdown_ntf, this);
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
	if (find_servicenode_byiid(sid) != 0)
		sid = make_next_serviceiid_fix();

	ServiceNodeInfo pnode;
	pnode.iid = sid;
	pnode.type = ctype;
	pnode.token = (S_INT_64)OSSystem::mOS->GetTimestamp();
	pnode.ip = req->ip().c_str();
	pnode.port = req->port();
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

		pLink->set_linkbase_info(pnode.iid, pnode.token, pnode.extparams);
		//关联node
		pLink->set_node(pnode);

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//登记到服务连接映射表
		service_mth_links_.regist_onlinelink(pLink);
	}

	pLink->registinfo_tolog(true);

	regist_one_service(pnode);

	Erk_ServiceRegist_ack *ack = new Erk_ServiceRegist_ack();
	ack->set_result(0);
	ack->set_iid(sid);
	ack->set_token( pnode.token);
	ack->set_eurekaiid(svrApp.get_eurekactrl()->get_myself().iid);
	ack->set_eurekatoken(svrApp.get_eurekactrl()->get_myself().token);

	pLink->send_to_service(ack);
}

void ServiceRegisterCtrl::on_mth_servicebind_req(NetProtocol* pro, bool& autorelease, void* session)
{
	Erk_ServiceBind_req* req = reinterpret_cast<Erk_ServiceBind_req*>(pro->msg_);
	EurekaSession* psession = reinterpret_cast<EurekaSession*>(session);

	std::string siid = std::to_string(req->iid());

	Erk_ServiceBind_ack* ack = new Erk_ServiceBind_ack();
	ack->set_result(0);

	ServiceNodeInfo* pnode = find_servicenode_byiid(req->iid());

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE || pnode == 0)
	{
		ack->set_result(1);

		SProtocolHead head = pro->head_;
		head.from_type_ = head.to_type_;
		head.to_type_ = pro->head_.from_type_;
		psession->send_to_service( head, ack);

		psession->force_close();
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

			psession->force_close();
			return;
		}

		svrApp.remove_waitsession_no_mutex(psession);

		pLink = service_mth_links_.ask_free_link();

		//关联node
		pLink->set_node(*pnode);

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
	Erk_ServiceSubscribe_req* req = dynamic_cast<Erk_ServiceSubscribe_req*>(pro->msg_);

	//更新redis心跳信息
	S_TIMESTAMP tnow = OSSystem::mOS->GetTimestamp();
	RedisClient* redis = svrApp.get_redisclient();
	{
		std::string skey = redis->build_rediskey(SERVICE_DETAIL_INFO, req->myiid());
		std::string stype = "";
		if (redis->get_hashmember(skey.c_str(), SERVICE_DETAIL_SVRTYPE, stype))
		{
			skey = redis->build_rediskey(SERVICE_MAINTNCE, stype.c_str());
			S_INT_64 lasttime = ((S_INT_64)tnow);
			redis->add_zset(skey.c_str(), std::to_string(req->myiid()).c_str(), lasttime, UpdateType::EXIST);
		}
	}

	//-------------------------协议同步-----------------------
	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(req->myiid());
	if (plink == 0)
		return;

	//无订阅回复一个空的协议
	if (req->svr_type_size() == 0)
	{
		Erk_ServiceSubscribe_ntf* ack = new Erk_ServiceSubscribe_ntf();
		ack->set_myiid(req->myiid());
		ack->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_NONE);

		plink->send_to_service(ack);

		return;
	}

	for (int ii = 0; ii < req->svr_type_size(); ++ii)
	{
		const Erk_ServiceSubscribe_req_svrinfo& sinfo = req->svr_type(ii);
		NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)sinfo.svr_type();
		if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
			continue;

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
		ack->set_svr_type(sinfo.svr_type());

		for (std::set<S_INT_64>::iterator iter = offline.begin(); iter != offline.end(); ++iter)
		{
			ack->add_offsvrs((*iter));
		}
		for (std::list<const ServiceNodeInfo*>::iterator iter = onlines.begin(); iter != onlines.end(); ++iter)
		{
			const ServiceNodeInfo* pinfo = (*iter);

			PRO::ServerNode* pnod = ack->add_newsvrs();
			pnod->set_iid(pinfo->iid);
			pnod->set_token(pinfo->token);
			pnod->set_ip(pinfo->ip);
			pnod->set_port(pinfo->port);

			if (pinfo->extparams.size() > 0)
			{
				google::protobuf::Map<std::string, std::string>& kvs = *(pnod->mutable_exts());

				for( boost::unordered_map<std::string, std::string>::const_iterator xiter = pinfo->extparams.cbegin(); 
					xiter != pinfo->extparams.cend(); ++xiter)
				{
					kvs[xiter->first] = xiter->second;
				}
			}
		}

		plink->send_to_service(ack);
	}
}

void ServiceRegisterCtrl::on_mth_servicesubscribe_ntf(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_servicesync_ntf(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_routersubscribe_req(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_routersubscribe_ntf(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_routeronline_req(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_routeronline_ntf(NetProtocol* pro, bool& autorelease)
{

}

void ServiceRegisterCtrl::on_mth_serviceshutdown_ntf(NetProtocol* pro, bool& autorelease)
{

}
