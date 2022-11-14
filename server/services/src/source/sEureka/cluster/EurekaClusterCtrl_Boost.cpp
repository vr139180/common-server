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

#include "sEurekaApp.h"

USED_REDISKEY_GLOBAL_NS

bool EurekaClusterCtrl::boot_ctrl()
{
	//初始化信息
	this->is_boosted_ = false;
	this->last_eureka_iid_ = 0;
	this->myself_.ismaster = false;
	this->myself_.iid = 0;
	this->eureka_master_iid_ = 0;

	//必须首先注册自己到系统
	myself_.ip = ConfigHelper::instance().get_ip();
	myself_.port = ConfigHelper::instance().get_port();
	myself_.token = (S_INT_64)OSSystem::mOS->GetTimestamp();

	bool iammaster = false;
	//获取master节点信息
	RedisClient* redis = svrApp.get_redisclient();
	S_INT_64 masterid = 0;
	EurekaNodeInfo masterNode;
	if (!redis->get_hashmember_ul(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_IID, masterid))
	{
		iammaster = true;
	}
	else
	{
		//是否是本服务重复启动
		if (!redis->get_hashobject(EUREKA_MASTER_NODE, FIELD_MASTER_NODE_SVR, masterNode))
		{
			svrApp.quit_app();
			return false;
		}

		if (masterNode.ip == myself_.ip && masterNode.port == myself_.port)
			iammaster = true;
	}

	if (iammaster)
	{
		make_me_masternode();

		//标记为启动成功
		this->mark_boosted();

		svrApp.on_notify_boosted();

		return true;
	}

	//保存到列表中
	slaver_regist_to_master(masterNode);
	
	return true;
}

void EurekaClusterCtrl::make_me_masternode()
{
	//没有master节点,设置自己为master
	myself_.ismaster = true;
	if( !is_boosted())
		myself_.iid = this->make_next_eurekaiid();

	this->eureka_master_iid_ = myself_.iid;

	//保存到列表中
	if( !is_eureka_node( myself_.iid))
		eureka_nodes_[myself_.iid] = myself_;

	//保存master节点信息
	if (!update_redis_masterinfo(0))
	{
		svrApp.quit_app();
		return;
	}

	if (!is_boosted())
		return;

	//中间切换master
}

void EurekaClusterCtrl::master_regist_one_slaver(EurekaNodeInfo& info)
{
	if (!is_eureka_node(info.iid))
	{
		//保存slaver节点
		eureka_nodes_[info.iid] = info;
	}
}

void EurekaClusterCtrl::master_sync_all_to_slaver( EurekaLinkFrom* from)
{
	//to from node
	//同步全量eureka节点信息
	Erk_EurekaUpdate_ntf *ntf = new Erk_EurekaUpdate_ntf();
	ntf->set_fulleurekas(true);
	ntf->set_masteriid(eureka_master_iid_);
	ntf->set_eureka_seed(last_eureka_iid_);
	ntf->set_service_seed(svrApp.get_servicectrl()->get_serviceiid_seed());

	for (boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator iter = eureka_nodes_.begin(); iter != eureka_nodes_.end(); ++iter)
	{
		EurekaNodeInfo& nod = iter->second;

		PRO::EurekaServerNode* pd = ntf->add_online();
		nod.copyto(pd);
	}

	from->send_to_eureka(ntf);

	//所有注册的服务
	BasicProtocol* allsvrs = svrApp.get_servicectrl()->master_syncall_servicenodes();
	from->send_to_eureka(allsvrs);
}

void EurekaClusterCtrl::master_notify_change_to_slaver(EurekaLinkFrom* notify)
{
	//to other slaver
	Erk_EurekaUpdate_ntf *ntf2 = new Erk_EurekaUpdate_ntf();
	std::unique_ptr<Erk_EurekaUpdate_ntf> ptr(ntf2);

	ntf2->set_fulleurekas(false);
	ntf2->set_masteriid(eureka_master_iid_);
	ntf2->set_eureka_seed(last_eureka_iid_);
	ntf2->set_service_seed(svrApp.get_servicectrl()->get_serviceiid_seed());

	PRO::EurekaServerNode* pd = ntf2->add_online();
	EurekaNodeInfo& nod = notify->get_node();
	nod.copyto(pd);

	this->broadcast_to_eurekas(ntf2, notify);

	//to 所有注册的服务
	svrApp.get_servicectrl()->broadcast_to_allsvrs(ntf2);
}

void EurekaClusterCtrl::master_invalid_one_slaver(S_INT_64 nodiid)
{

}

void EurekaClusterCtrl::slaver_regist_to_master(EurekaNodeInfo info)
{
	//注册到eureka节点列表中
	if (!is_eureka_node(info.iid))
	{
		//保存master节点
		eureka_nodes_[info.iid] = info;

		eureka_links_to_.add_linkto_node(&info);
	}

	//master节点编号
	this->eureka_master_iid_ = info.iid;

	//强制发起连接
	eureka_links_to_.connect_to();
}

void EurekaClusterCtrl::slaver_sync_eurekanodes(Erk_EurekaUpdate_ntf* ntf)
{
	if (ntf->fulleurekas())
	{
		boost::unordered_map<S_INT_64, S_INT_64> existiid;
		for (boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator iter = eureka_nodes_.begin(); iter != eureka_nodes_.end(); ++iter)
			existiid[iter->first] = iter->first;

		boost::unordered_map<S_INT_64, S_INT_64> alladds;

		//全量更新
		for (int ii = 0; ii < ntf->online_size(); ++ii)
		{
			const PRO::EurekaServerNode& nod = ntf->online(ii);

			alladds[nod.iid()] = nod.iid();

			if (is_eureka_node(nod.iid()))
				continue;

			//加入新的
			EurekaNodeInfo info;
			info = nod;

			eureka_nodes_[info.iid] = info;
			
			EurekaNodeInfo* pinfo = get_eureka_node(info.iid);
			//大的主动连接小的
			if (myself_.iid > pinfo->iid)
				eureka_links_to_.add_linkto_node(pinfo);
		}

		//马上发起连接
		eureka_links_to_.connect_to();

		//是否有删除的
		for (boost::unordered_map<S_INT_64, S_INT_64>::iterator iter = existiid.begin(); iter != existiid.end(); ++iter)
		{
			S_INT_64 iid = iter->first;
			boost::unordered_map<S_INT_64, S_INT_64>::iterator fiter = alladds.find(iid);
			//原来就有的
			if (fiter != alladds.end())
				continue;

			//被移除的，需要更新
			slaver_eurekanode_invalid(iid);
		}
	}
	else
	{
		//增量数据
		for (int ii = 0; ii < ntf->online_size(); ++ii)
		{
			const PRO::EurekaServerNode& nod = ntf->online(ii);

			if (is_eureka_node(nod.iid()))
				continue;

			//加入新的
			EurekaNodeInfo info;
			info = nod;

			eureka_nodes_[info.iid] = info;

			EurekaNodeInfo* pinfo = get_eureka_node(info.iid);
			//大的主动连接小的
			if (myself_.iid > pinfo->iid)
				eureka_links_to_.add_linkto_node(pinfo);
		}

		//马上发起连接
		eureka_links_to_.connect_to();

		//是否有删除的
		for (int ii = 0; ii < ntf->offline_size(); ++ii)
		{
			S_INT_64 iid = ntf->offline(ii);
			//被移除的，需要更新
			slaver_eurekanode_invalid(iid);
		}
	}
}

void EurekaClusterCtrl::slaver_eurekanode_invalid(S_INT_64 nodiid)
{
	eureka_nodes_.erase(nodiid);

	if (myself_.iid > nodiid)
		eureka_links_to_.remove_linkto_node(nodiid);
	else
		eureka_links_from_.remove_linkfrom_node(nodiid);
}
