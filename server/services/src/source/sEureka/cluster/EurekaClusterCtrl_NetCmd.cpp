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
}

void EurekaClusterCtrl::on_eurekabind_req(NetProtocol* pro, bool& autorelease, void* session)
{
	EurekaSession* pes = reinterpret_cast<EurekaSession*>(session);
	Erk_EurekaBind_req* req = reinterpret_cast<Erk_EurekaBind_req*>(pro->msg_);

	if (is_eureka_exist(req->iid()) || (!is_boosted()))
	{
		//ǿ�ƹҶ�
		pes->force_close();

		logError(out_runtime, "one eureka:%lld bind request failed. already binded.", req->iid());
		return;
	}
	
	//��������
	EurekaLinkFrom* pfrom = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(pes);

		if (free_links_from_.size() == 0)
		{
			pfrom = new EurekaLinkFrom();
			links_from_.push_back(pfrom);
		}
		else
		{
			std::set<EurekaLinkFrom*>::iterator fiter = free_links_from_.begin();
			pfrom = (*fiter);
			free_links_from_.erase(fiter);
		}

		EurekaNodeInfo* pnode = new EurekaNodeInfo();
		pnode->iid = req->iid();
		pnode->token = req->token();
		pnode->ip = req->ip().c_str();
		pnode->port = req->port();

		//����node
		pfrom->set_node(pnode);

		pes->auth();
		pfrom->set_session(pes);
		pes->set_netlinkbase(pfrom);

		//ע�ᵽ���ӽڵ���
		link_nodes_[pfrom->get_iid()] = (IEurekaNodeLink*)pfrom;
	}

	if (pfrom)
	{
		pfrom->registinfo_tolog(true);

		Erk_EurekaBind_ack *ack = new Erk_EurekaBind_ack();
		ack->set_result(0);
		pfrom->send_to_eureka(ack);
	}
}

void EurekaClusterCtrl::on_service_eureka_sync(NetProtocol* message, bool& autorelease)
{
	Erk_Eureka_sync* syn = dynamic_cast<Erk_Eureka_sync*>(message->msg_);

	//����ͬ�������ṩ����������Ϣ
	std::set<S_INT_64> offlines;
	for (int ii = 0; ii < syn->exists_size(); ++ii)
	{
		offlines.insert(syn->exists(ii));
	}

	std::list<EurekaNodeInfo*> onlines;
	for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = redis_nodes.begin(); iter != redis_nodes.end(); ++iter)
	{
		S_INT_64 sid = iter->first;
		std::set<S_INT_64>::iterator fiter = offlines.find(sid);
		if (fiter == offlines.end())
		{
			onlines.push_back(iter->second);
		}
		else
		{
			offlines.erase(fiter);
		}
	}

	Erk_EurekaUpdate_ntf* ntf = new Erk_EurekaUpdate_ntf();
	ntf->set_myiid(syn->myiid());

	for (std::list<EurekaNodeInfo*>::iterator iter = onlines.begin(); iter != onlines.end(); ++iter)
	{
		EurekaNodeInfo* pinfo = (*iter);
		
		EurekaServerNode* pn = ntf->add_online();
		pn->set_iid(pinfo->iid);
		pn->set_token(pinfo->token);
		pn->set_ip(pinfo->ip);
		pn->set_port(pinfo->port);
	}
	for (std::set<S_INT_64>::iterator iter = offlines.begin(); iter != offlines.end(); ++iter)
	{
		S_INT_64 sid = (*iter);
		ntf->add_offline(sid);
	}

	//����,·�ɵ�service
	NETCMD_FUN_MAP fun = boost::bind(&ServiceRegisterCtrl::on_mth_message_route_to_service, svrApp.get_servicectrl(), 
		boost::placeholders::_1, boost::placeholders::_2);

	SProtocolHead head = message->head_;
	head.from_type_ = head.to_type_;
	head.to_type_ = message->head_.from_type_;
	NetProtocol *pro = new NetProtocol(head, ntf);

	NetCommand *pcmd = new NetCommand(pro, fun);

	svrApp.regist_syscmd(pcmd);
}
