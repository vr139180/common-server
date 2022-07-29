#include "services/ServiceRegisterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

void ServiceRegisterCtrl::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::SVR_GATEBINDHOME_REQ, &ServiceRegisterCtrl::on_mth_gatebindhome_req, this);
	REGISTERMSG(ERK_PROTYPE::SVR_GATEBINDHOME_CONFIRM, &ServiceRegisterCtrl::on_mth_gatebindhome_confirm, this);
}

void ServiceRegisterCtrl::on_mth_serviceregist_req(BasicProtocol* pro, bool& autorelease, void* session)
{
	Erk_ServiceRegist_req* req = dynamic_cast<Erk_ServiceRegist_req*>(pro);
	EurekaSession* psession = reinterpret_cast<EurekaSession*>(session);

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
	{
		Erk_ServiceRegist_ack* ack = new Erk_ServiceRegist_ack();
		ack->set_result(1);
		psession->send_protocol(ack);

		//参数错误强制关闭
		psession->force_close();
		return;
	}

	RedisClient* redis = svrApp.get_redisclient();
	//生成服务iid,全局唯一
	S_INT_64 sid = redis->gen_uuid_from_redis(GLOBAL_IDGEN_HKEY, GLOBAL_IDGEN_SERVICE_F);

	ServiceNodeInfo* pnode = new ServiceNodeInfo();
	pnode->iid = sid;
	pnode->type = ctype;
	pnode->token = (S_INT_64)OSSystem::mOS->GetTimestamp();
	pnode->ip = req->ip().c_str();
	pnode->port = req->port();
	if (req->exts_size() > 0)
	{
		const google::protobuf::Map<std::string,std::string>& kvs = req->exts();
		for (google::protobuf::Map<std::string, std::string>::const_iterator iter = kvs.cbegin(); 
			iter != kvs.cend(); ++iter)
		{
			pnode->extparams[iter->first] = iter->second;
		}
	}

	ServiceLinkFrom* pLink = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(psession);

		pLink = service_mth_links_.ask_free_link();

		//关联node
		pLink->set_node(pnode);

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//登记到服务连接映射表
		service_mth_links_.regist_onlinelink(pLink);
	}

	//注册到redis
	redis_serviceregist_do1(pnode);

	pLink->registinfo_tolog(true);

	Erk_ServiceRegist_ack *ack = new Erk_ServiceRegist_ack();
	ack->set_result(0);
	ack->set_iid(sid);
	ack->set_token( pnode->token);
	ack->set_eurekaiid(svrApp.get_eurekactrl()->get_myself().iid);
	ack->set_eurekatoken(svrApp.get_eurekactrl()->get_myself().token);

	pLink->send_protocol(ack);
}

void ServiceRegisterCtrl::on_mth_serviceregist_confirm(BasicProtocol* pro, bool& autorelease)
{
	Erk_ServiceRegist_Confirm* req = dynamic_cast<Erk_ServiceRegist_Confirm*>(pro);
	
	redis_serviceregist_do2( req->iid());
}

void ServiceRegisterCtrl::on_mth_servicebind_req(BasicProtocol* pro, bool& autorelease, void* session)
{
	Erk_ServiceBind_req* req = reinterpret_cast<Erk_ServiceBind_req*>(pro);
	EurekaSession* psession = reinterpret_cast<EurekaSession*>(session);

	std::string siid = std::to_string(req->iid());

	Erk_ServiceBind_ack* ack = new Erk_ServiceBind_ack();
	ack->set_result(0);

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
	{
		ack->set_result(1);
		psession->send_protocol(ack);

		psession->force_close();
		return;
	}

	RedisClient* redis = svrApp.get_redisclient();

	ServiceNodeInfo* pnode = new ServiceNodeInfo();
	std::string catkey = redis->build_rediskey(SERVICE_CATEGORY, NetServiceType::to_string(ctype).c_str());
	if (redis->get_hashobject<ServiceNodeInfo>(catkey.c_str(), siid.c_str(), *pnode) == false)
	{
		delete pnode;

		ack->set_result(1);
		psession->send_protocol(ack);

		psession->force_close();
		return;
	}

	std::string skey = redis->build_rediskey(SERVICE_DETAIL_INFO, req->iid());
	S_INT_64 token = 0;
	redis->get_hashmember_ul(skey.c_str(), SERVICE_DETAIL_TOKEN, token);

	if (req->token() != token)
	{
		ack->set_result(1);
		psession->send_protocol(ack);

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
			psession->send_protocol(ack);
			psession->force_close();
			return;
		}

		svrApp.remove_waitsession_no_mutex(psession);

		pLink = service_mth_links_.ask_free_link();

		//关联node
		pLink->set_node(pnode);

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		service_mth_links_.regist_onlinelink(pLink);
	}

	//注册到redis
	skey = redis->build_rediskey(EUREKA_CLUSTER_SVRBIND, svrApp.get_eurekactrl()->get_myself().iid);
	redis->add_set(skey.c_str(), siid.c_str());

	pLink->registinfo_tolog(true);
	pLink->send_protocol(ack);
}

void ServiceRegisterCtrl::on_mth_message_route_to_service( BasicProtocol* pro, bool& autorelease, int sid)
{
	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(sid);
	if (plink == 0)
		return;

	//接管 pro的内存释放
	autorelease = false;

	plink->send_protocol(pro);
}

void ServiceRegisterCtrl::on_mth_servicesubscribe_req(BasicProtocol* pro, bool& autorelease)
{
	Erk_ServiceSubscribe_req* req = dynamic_cast<Erk_ServiceSubscribe_req*>(pro);

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
		Erk_ServiceSubscribe_ack* ack = new Erk_ServiceSubscribe_ack();
		ack->set_myiid(req->myiid());
		ack->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_NONE);

		plink->send_protocol(ack);

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
		const std::list<ServiceNodeInfo*>& pnodes = get_service_meth_meta_oftype(ctype);
		for (std::list<ServiceNodeInfo*>::const_iterator iter = pnodes.begin(); iter != pnodes.end(); ++iter)
		{
			const ServiceNodeInfo* pinfo = (*iter);
			std::set<S_INT_64>::iterator fiter = offline.find(pinfo->iid);
			if (fiter == offline.end())
			{
				//过滤同类型服务的,相同ip port的服务
				if (checkMyIpPort)
				{
					if (req->myip() == pinfo->ip && req->myport() == pinfo->port)
						continue;
				}

				onlines.push_back(pinfo);
			}
			else
				offline.erase(fiter);
		}

		Erk_ServiceSubscribe_ack* ack = new Erk_ServiceSubscribe_ack();
		ack->set_myiid(req->myiid());
		ack->set_svr_type(sinfo.svr_type());

		for (std::set<S_INT_64>::iterator iter = offline.begin(); iter != offline.end(); ++iter)
		{
			ack->add_offline((*iter));
		}
		for (std::list<const ServiceNodeInfo*>::iterator iter = onlines.begin(); iter != onlines.end(); ++iter)
		{
			const ServiceNodeInfo* pinfo = (*iter);

			PRO::ServerNode* pnod = ack->add_online();
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

		plink->send_protocol(ack);
	}
}

void ServiceRegisterCtrl::on_mth_gatebindhome_req(BasicProtocol* pro, bool& autorelease)
{
	Svr_GateBindHome_req* req = dynamic_cast<Svr_GateBindHome_req*>(pro);
	ServiceLinkFrom* plink = service_mth_links_.get_servicelink_byiid(req->gateiid());

	logDebug(out_runtime, "recevie gate bind home request....");

	if (plink == 0 || plink->get_token() != req->gatetoken()) {
		logWarn(out_runtime, "gate service[%ld:%ld] not exist", req->gateiid(), req->gatetoken());
		return;
	}

	Svr_GateBindHome_ack* ack = new Svr_GateBindHome_ack();
	ack->set_result(1);
	ack->set_gateiid(req->gateiid());

	int ret = 0;
	S_INT_64 bindtoken = OSSystem::mOS->GetTimestamp();
	ServiceNodeInfo node;
	ret = redis_gatebindhome_do(req->gateiid(), req->gatetoken(), bindtoken, node);
	ack->set_result(ret);
	if (ret == 0)
	{
		ack->set_homeiid(node.iid);
		ack->set_hometoken(node.token);
		ack->set_homeip(node.ip.c_str());
		ack->set_homeport(node.port);
		ack->set_bindtoken(bindtoken);
	}

	plink->send_protocol(ack);
}

void ServiceRegisterCtrl::on_mth_gatebindhome_confirm(BasicProtocol* pro, bool& autorelease)
{
	Svr_GateBindHome_confirm* confirm = dynamic_cast<Svr_GateBindHome_confirm*>(pro);
	
	redis_gatebindhome_confirm(confirm->gateiid(), confirm->homeiid(), confirm->bindtoken());
}