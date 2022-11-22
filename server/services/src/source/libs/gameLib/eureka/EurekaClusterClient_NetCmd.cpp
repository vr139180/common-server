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
	REGISTERMSG(ERK_PROTYPE::ERK_MASTERCHANGE_NTF, &EurekaClusterClient::on_eurekamaster_change_ntf, this);
	REGISTERMSG(ERK_PROTYPE::SVR_ROUTERONLINE_NTF, &EurekaClusterClient::on_routeronline_ntf, this);
}

void EurekaClusterClient::on_eurekaupdate_ntf(NetProtocol* message, bool& autorelease)
{
	Erk_EurekaUpdate_ntf *ntf = dynamic_cast<Erk_EurekaUpdate_ntf*>(message->msg_);

	//新增增量
	for (int ind = 0; ind < ntf->online_size(); ++ind)
	{
		const EurekaServerNode& node = ntf->online(ind);
		if (is_eurekanode_exist(node.iid()))
			continue;

		//处理新增加的节点
		EurekaNodeInfo pinfo;
		pinfo = node;

		eureka_nodes_[pinfo.iid] = pinfo;

		//eureka已经在线
		if (is_eurekanode_connected(pinfo.iid))
			continue;

		//全新的eureka，加入连接缓冲
		EurekaClusterLink* pconn = 0;
		//放入连接节点
		if (free_links_.size() == 0)
		{
			EurekaClusterLink* pc = new EurekaClusterLink(this, pinfo);
			clusterlinks_.push_back(pc);
			pconn = pc;
		}
		else
		{
			std::set<EurekaClusterLink*>::iterator fiter = free_links_.begin();
			pconn = (*fiter);
			free_links_.erase(fiter);

			pconn->reset(&pinfo);
		}

		wait_links_.insert(pconn);
		logDebug(out_runtime, "slaver eureka node:%lld wait to connecting", pconn->get_iid());
	}

	//删除下线的服务，但是不直接断开连接。通过错误重连时来自动维护
	for (int ind = 0; ind < ntf->offline_size(); ++ind)
	{
		S_INT_64 sid = ntf->offline(ind);
		boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(sid);
		if (fiter == eureka_nodes_.end())
			continue;

		eureka_nodes_.erase(fiter);
	}
}

void EurekaClusterClient::on_service_subscribe_ntf(NetProtocol* message, bool& autorelease)
{
	Erk_ServiceSubscribe_ntf* ack = dynamic_cast<Erk_ServiceSubscribe_ntf*>(message->msg_);

	NETSERVICE_TYPE type = (NETSERVICE_TYPE)ack->svr_type();
	SERVICENODE_TYPE& s_nodes = get_servicenodes_by_type( type);

	std::list<ServiceNodeInfo*> newiids;
	//在线服务增量信息
	for (int ind = 0; ind < ack->newsvrs_size(); ++ind)
	{
		const ServerNode& node = ack->newsvrs(ind);
		SERVICENODE_TYPE::iterator fiter = s_nodes.find(node.iid());
		if (fiter != s_nodes.end())
			continue;

		//新增服务
		ServiceNodeInfo* ps = new ServiceNodeInfo();
		ps->copy_from(&node);

		s_nodes[ps->iid] = ps;

		newiids.push_back(ps);
	}

	//下线服务
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
	Erk_RouterSubscribe_ntf* ack = dynamic_cast<Erk_RouterSubscribe_ntf*>(message->msg_);

	NETSERVICE_TYPE type = (NETSERVICE_TYPE)ack->svr_type();

	std::list<S_INT_64> alliids;
	//在线服务增量信息
	for (int ind = 0; ind < ack->svriids_size(); ++ind)
	{
		S_INT_64 node = ack->svriids(ind);
		alliids.push_back(node);
	}

	if (app_proxy_)
	{
		app_proxy_->mth_notify_routerbalance_new(type, alliids);
	}
}

void EurekaClusterClient::on_eurekamaster_change_ntf(NetProtocol* message, bool& autorelease)
{
	Erk_MasterChange_ntf* ack = dynamic_cast<Erk_MasterChange_ntf*>(message->msg_);

	if (master_eureka_iid_ != ack->newmaster() || master_eureka_token_ != ack->mastertoken())
	{
		master_eureka_iid_ = ack->newmaster();
		master_eureka_token_ = ack->mastertoken();
		master_link_ = get_eurekalink_byiid(master_eureka_iid_);

		if( master_link_ != 0)
			subscribe_to_masternode();
	}
}

void EurekaClusterClient::on_routeronline_ntf(NetProtocol* pro, bool& autorelease)
{
	Svr_RouterOnline_ntf* ntf = dynamic_cast<Svr_RouterOnline_ntf*>(pro->msg_);
}