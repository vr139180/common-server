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

#include "gameLib/eureka/EurekaClusterClient.h"

#include <cmsLib/system/CommandBase.h>
#include "gameLib/protobuf/Proto_all.h"
#include <gameLib/LogExt.h>

USE_PROTOCOL_NAMESPACE

void EurekaClusterClient::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF, &EurekaClusterClient::on_eurekaupdate_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_SERVICESUBSCRIBE_NTF, &EurekaClusterClient::on_service_subscribe_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_ROUTERSUBSCRIBE_NTF, &EurekaClusterClient::on_router_subscribe_ntf, this);
}

void EurekaClusterClient::on_eurekaupdate_ntf(NetProtocol* message, bool& autorelease)
{
	Erk_EurekaUpdate_ntf *ntf = dynamic_cast<Erk_EurekaUpdate_ntf*>(message->msg_);

	//��������
	for (int ind = 0; ind < ntf->online_size(); ++ind)
	{
		const EurekaServerNode& node = ntf->online(ind);
		if (is_eurekanode_exist(node.iid()))
			continue;

		//���������ӵĽڵ�
		EurekaNodeInfo* pinfo = new EurekaNodeInfo();
		pinfo->iid = node.iid();
		pinfo->token = node.token();
		pinfo->ip = node.ip();
		pinfo->port = node.port();

		eureka_nodes_[pinfo->iid] = pinfo;

		//eureka�Ѿ�����
		if (is_eurekanode_connected(pinfo->iid))
			continue;

		//ȫ�µ�eureka���������ӻ���
		EurekaClusterLink* pconn = 0;
		//�������ӽڵ�
		if (free_links_.size() == 0)
		{
			EurekaClusterLink* pc = new EurekaClusterLink(this, pinfo->clone());
			clusterlinks_.push_back(pc);
			pconn = pc;
		}
		else
		{
			std::set<EurekaClusterLink*>::iterator fiter = free_links_.begin();
			pconn = (*fiter);
			free_links_.erase(fiter);

			pconn->reset(pinfo->clone());
		}

		wait_links_.insert(pconn);
		logDebug(out_runtime, "recv a new eureka node:%lld wait to connecting", pconn->get_iid());
	}

	//ɾ�����ߵķ��񣬵��ǲ�ֱ�ӶϿ����ӡ�ͨ����������ʱ���Զ�ά��
	for (int ind = 0; ind < ntf->offline_size(); ++ind)
	{
		S_INT_64 sid = ntf->offline(ind);
		boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator fiter = eureka_nodes_.find(sid);
		if (fiter == eureka_nodes_.end())
			continue;

		EurekaNodeInfo* pp = fiter->second;
		eureka_nodes_.erase(fiter);

		delete pp;
	}
}

void EurekaClusterClient::on_service_subscribe_ntf(NetProtocol* message, bool& autorelease)
{
	Erk_ServiceSubscribe_ntf* ack = dynamic_cast<Erk_ServiceSubscribe_ntf*>(message->msg_);

	NETSERVICE_TYPE type = (NETSERVICE_TYPE)ack->svr_type();
	SERVICENODE_TYPE& s_nodes = get_servicenodes_by_type( type);

	std::list<ServiceNodeInfo*> newiids;
	//���߷���������Ϣ
	for (int ind = 0; ind < ack->newsvrs_size(); ++ind)
	{
		const ServerNode& node = ack->newsvrs(ind);
		SERVICENODE_TYPE::iterator fiter = s_nodes.find(node.iid());
		if (fiter != s_nodes.end())
			continue;

		//��������
		ServiceNodeInfo* ps = new ServiceNodeInfo();
		ps->iid = node.iid();
		ps->token = node.token();
		ps->ip = node.ip();
		ps->port = node.port();
		if (node.exts_size() > 0)
		{
			const google::protobuf::Map<std::string, std::string>& kvs = node.exts();
			for (google::protobuf::Map<std::string, std::string>::const_iterator xiter = kvs.cbegin();
				xiter != kvs.cend(); ++xiter)
			{
				ps->extparams[xiter->first] = xiter->second;
			}
		}

		s_nodes[ps->iid] = ps;

		newiids.push_back(ps);
	}

	//���߷���
	std::list<S_INT_64>	deliids;
	for (int ind = 0; ind < ack->offsvrs_size(); ++ind)
	{
		S_INT_64 sid = ack->offsvrs(ind);
		SERVICENODE_TYPE::iterator fiter = s_nodes.find(sid);
		if (fiter == s_nodes.end())
			continue;

		deliids.push_back(sid);

		ServiceNodeInfo* pp = fiter->second;
		s_nodes.erase(fiter);

		delete pp;
	}

	if (app_proxy_)
	{
		app_proxy_->mth_notify_servicenode_new(type, newiids, deliids);
	}
}

void EurekaClusterClient::on_router_subscribe_ntf(NetProtocol* message, bool& autorelease)
{

}
