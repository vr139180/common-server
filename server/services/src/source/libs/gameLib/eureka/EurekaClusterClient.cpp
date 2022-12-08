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

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/system/CommandBase.h>
#include <cmsLib/httpcurl/HttpClient.h>
#include <gameLib/LogExt.h>
#include "gameLib/protobuf/Proto_all.h"

EurekaClusterClient& EurekaClusterClient::instance()
{
	static EurekaClusterClient s_eureka;
	return s_eureka;
}

bool EurekaClusterClient::get_eureka_masterinfo( const char* eurekaurl, EurekaNodeInfo& info)
{
	HttpClient	http_client;

	HttpUrl url(eurekaurl);
	HttpResponse resp;
	HttpStringWrite writer;
	if (http_client.HttpGET(url, resp, writer))
	{
		try {
			boost::json::parse_options opt;
			opt.allow_comments = true;
			opt.allow_trailing_commas = true;

			boost::json::error_code ec;
			boost::json::value root = boost::json::parse( writer.GetBody(), ec, {}, opt);
			if (ec.failed())
				return false;

			boost::json::object& obj = root.as_object();
			int result = JSONUtil::get_value<int>(obj, "code", 1);
			if (result != 0)
				return false;

			if (!obj.contains("data"))
				return false;

			boost::json::object& data = obj.at("data").as_object();

			info.iid = JSONUtil::get_int64(data, "iid");
			info.ip = JSONUtil::get_string(data, "ip");
			info.port = JSONUtil::get_value<int>(data, "port", 4001);
			info.token = JSONUtil::get_int64(data, "token");

			return true;
		}
		catch (...) {
			return false;
		}
	}
	else
	{
		return false;
	}
}

EurekaClusterClient::EurekaClusterClient():
	service_iid_(0)
	, token_(0)
	, cur_state_(EurekaState::Eureka_WaitInit)
	, master_eureka_iid_(0)
	, master_eureka_token_(0)
	, app_proxy_(0)
	, is_router_node_(false)
	, master_link_(0)
	, cur_link_index_(0)
{
}

EurekaClusterClient::~EurekaClusterClient()
{
}

void EurekaClusterClient::release()
{
	master_link_ = 0;
	online_links_.clear();
	auth_links_.clear();
	wait_links_.clear();
	free_links_.clear();

	for (std::list<EurekaClusterLink*>::iterator iter = clusterlinks_.begin(); iter != clusterlinks_.end(); ++iter)
	{
		EurekaClusterLink* plink = (*iter);
		plink->force_close();
		delete (*iter);
	}
	clusterlinks_.clear();
}

void EurekaClusterClient::init(IEurekaClientIntegrate* app, NETSERVICE_TYPE type
	, const char* myip, int myport, EurekaServerExtParam extpms
	, EurekaNodeInfo& eureka
	, std::list< NETSERVICE_TYPE>& subscribe_service
	, std::list< NETSERVICE_TYPE>& subscribe_balance
	, bool isrouter)
{
	this->cur_state_ = EurekaState::Eureka_WaitInit;

	this->app_proxy_ = app;
	this->svrtype_ = type;
	this->myip_ = myip;
	this->myport_ = myport;
	this->extpms_ = extpms;
	this->subscribe_services_ = subscribe_service;
	this->subscribe_balance_ = subscribe_balance;
	this->is_router_node_ = isrouter;

	for (std::list< NETSERVICE_TYPE>::iterator iter = subscribe_services_.begin(); iter != subscribe_services_.end(); ++iter)
	{
		service_nodes_[(*iter)] = SERVICENODE_TYPE();
	}

	//初始化消息处理映射
	this->InitNetMessage();

	EurekaNodeInfo pnode;
	pnode = eureka;
	pnode.ismaster = true;
	EurekaClusterLink* plink = new EurekaClusterLink(this, pnode);

	//plink不会释放，多余的放到free作为缓存使用
	clusterlinks_.push_back(plink);
	wait_links_.insert(plink);
}

void EurekaClusterClient::uninit()
{
	release();
}

void EurekaClusterClient::send_mth_protocol(BasicProtocol* pro, bool balance)
{
	int num = (int)online_links_.size();
	if (num == 0)
	{
		delete pro;
		return;
	}

	if (cur_link_index_ >= num)
		cur_link_index_ = 0;
	else if (cur_link_index_ < 0)
		cur_link_index_ = 0;

	online_links_[cur_link_index_]->send_to_eureka(pro);

	if( balance)
		++cur_link_index_;
}

void EurekaClusterClient::send_to_master(BasicProtocol* pro)
{
	if (master_link_ == 0)
	{
		delete pro;
		return;
	}

	master_link_->send_to_eureka(pro);
}

void EurekaClusterClient::regist_command(CommandBase *p)
{
	if (app_proxy_ == 0) {
		delete p;
		return;
	}

	app_proxy_->regist_eurekacommand(p);
}

void EurekaClusterClient::regist_timer()
{
	if (app_proxy_ == 0)
		return;

	auto_connect_tk_ = app_proxy_->add_apptimer_proxy(3 * 1000, boost::BOOST_BIND(&EurekaClusterClient::auto_connect_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));

	app_proxy_->add_apptimer_proxy(8 * 1000, boost::BOOST_BIND(&EurekaClusterClient::service_alive_timer, this,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
}

void EurekaClusterClient::auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	//自动连接
	if (wait_links_.size() > 0)
	{
		//未完成注册，允许重连
		//完成注册，但未完成第二步骤的确认，不能重连
		if( (!is_registed()) || is_done())
		{
			//发起连接，放入等待认证队列
			for (std::set<EurekaClusterLink*>::iterator iter = wait_links_.begin(); iter != wait_links_.end(); ++iter)
			{
				EurekaClusterLink* plink = (*iter);
				plink->connect();

				auth_links_.insert(plink);
			}
			wait_links_.clear();
		}
	}
}

bool EurekaClusterClient::is_eurekanode_exist(S_INT_64 sid)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo>::iterator fiter = eureka_nodes_.find(sid);
	return fiter != eureka_nodes_.end();
}

bool EurekaClusterClient::is_eurekanode_connected(S_INT_64 sid)
{
	for (int ii = 0; ii < (int)online_links_.size(); ++ii)
	{
		EurekaClusterLink* plink = online_links_[ii];
		if (plink->get_iid() == sid)
			return true;
	}

	return false;
}

bool EurekaClusterClient::is_servicenode_exist(NETSERVICE_TYPE type, S_INT_64 sid)
{
	SERVICENODE_TYPE& nodes = get_servicenodes_by_type(type);

	SERVICENODE_TYPE::iterator fiter2 = nodes.find(sid);
	return fiter2 != nodes.end();
}

EurekaClusterClient::SERVICENODE_TYPE & EurekaClusterClient::get_servicenodes_by_type(NETSERVICE_TYPE type)
{
	boost::unordered_map<NETSERVICE_TYPE, SERVICENODE_TYPE >::iterator fiter = service_nodes_.find(type);
	if (fiter == service_nodes_.end())
	{
		service_nodes_[type] = SERVICENODE_TYPE();
		fiter = service_nodes_.find(type);
	}

	return fiter->second;
}

void EurekaClusterClient::on_link_disconnected(EurekaClusterLink* plink)
{
	auth_links_.erase(plink);
	wait_links_.erase(plink);

	{
		std::vector<EurekaClusterLink*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
		if (fiter != online_links_.end())
			online_links_.erase(fiter);
	}

	//3秒后重连
	if (is_registed())
	{
		if (master_link_ == plink)
		{
			master_link_ = 0;
			logInfo(out_runtime, "me lost connection to sEureka[iid:%ld ip:%s port:%d] master node",
				plink->get_node().iid, plink->get_node().ip.c_str(), plink->get_node().port);
		}

		if (is_eurekanode_exist(plink->get_iid()))
		{
			//有效的节点，则继续链接
			wait_links_.insert(plink);
		}
		else
		{
			//无效的节点，放入缓存
			free_links_.insert(plink);
		}

		if (online_links_.size() == 0)
		{
			app_proxy_->mth_eureka_losted();
		}
	}
	else
	{
		//未完成注册，需要重复的链接eureka
		wait_links_.insert(plink);
	}

	plink->force_close();
}

void EurekaClusterClient::on_link_regist_result( EurekaClusterLink* plink)
{
	auth_links_.erase(plink);

	//放入在线节点
	std::vector<EurekaClusterLink*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
	if (fiter == online_links_.end())
		online_links_.push_back(plink);

	//第一个需要手动缓存，后续的通过同步
	if (is_eurekanode_exist(plink->get_iid()) == false)
	{
		eureka_nodes_[plink->get_node().iid] = plink->get_node();
	}

	//连接成功
	this->cur_state_ = EurekaState::Eureka_Registed;

	//更新master信息
	this->master_eureka_iid_ = plink->get_node().iid;
	this->master_eureka_token_ = plink->get_node().token;
	this->master_link_ = plink;

	subscribe_to_masternode();

	if (is_router_node_)
	{
		//delay 5 second 发送routeronline_req
		app_proxy_->add_apptimer_proxy(3 * 1000, boost::BOOST_BIND(&EurekaClusterClient::router_autoconfirm_timer, this,
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3, boost::placeholders::_4));
	}
	else
	{
		this->cur_state_ = EurekaState::Eureka_Done;
		if (app_proxy_)
			app_proxy_->mth_service_registed(get_myiid());
	}
}

void EurekaClusterClient::on_link_bind_result( EurekaClusterLink* plink)
{
	auth_links_.erase(plink);

	//放入有效节点
	std::vector<EurekaClusterLink*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
	if (fiter == online_links_.end())
		online_links_.push_back(plink);

	//是否重新绑定的是master
	if (plink->get_node().iid == master_eureka_iid_ && plink->get_node().token == master_eureka_token_)
	{
		master_link_ = plink;

		subscribe_to_masternode();
	}
}

EurekaClusterLink* EurekaClusterClient::get_eurekalink_byiid(S_INT_64 iid)
{
	for (std::vector<EurekaClusterLink*>::iterator iter = online_links_.begin(); iter != online_links_.end(); ++iter)
	{
		EurekaClusterLink* plink = (*iter);
		if (plink->get_iid() == iid)
			return plink;
	}

	return 0;
}

void EurekaClusterClient::subscribe_to_masternode()
{
	//订阅请求
	//服务订阅
	if(subscribe_services_.size() > 0)
	{
		PRO::Erk_ServiceSubscribe_req* req = new PRO::Erk_ServiceSubscribe_req();
		req->set_myiid(get_myiid());
		req->set_mysvrtype((S_INT_32)svrtype_);

		for (std::list< NETSERVICE_TYPE>::iterator iter = subscribe_services_.begin(); iter != subscribe_services_.end(); ++iter)
		{
			PRO::Erk_ServiceSubscribe_req_svrinfo* sinfo = req->add_svr_type();
			sinfo->set_svr_type((S_INT_32)(*iter));

			boost::unordered_map<NETSERVICE_TYPE, SERVICENODE_TYPE >::iterator fiter = service_nodes_.find((*iter));
			if (fiter == service_nodes_.end())
				continue;

			const SERVICENODE_TYPE& snodes = fiter->second;
			for (SERVICENODE_TYPE::const_iterator iter2 = snodes.begin(); iter2 != snodes.end(); ++iter2)
			{
				sinfo->add_exits(iter2->first);
			}
		}

		this->send_to_master(req);
	}

	//router节点，订阅负载均衡信息
	if (subscribe_balance_.size() > 0)
	{
		PRO::Erk_RouterSubscribe_req* req = new PRO::Erk_RouterSubscribe_req();
		req->set_myiid(get_myiid());
		req->set_mysvrtype((S_INT_32)svrtype_);

		for (std::list< NETSERVICE_TYPE>::iterator iter = subscribe_balance_.begin(); iter != subscribe_balance_.end(); ++iter)
		{
			req->add_svr_types((S_INT_32)(*iter));
		}

		this->send_to_master(req);
	}
}

void EurekaClusterClient::router_autoconfirm_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	finish = true;

	cur_state_ = EurekaState::Eureka_Done;

	PRO::Erk_RouterOnline_req* req = new PRO::Erk_RouterOnline_req();
	req->set_myiid(get_myiid());
	req->set_mysvrtype((S_INT_32)svrtype_);

	this->send_to_master(req);

	app_proxy_->mth_service_registed(get_myiid());
}

void EurekaClusterClient::service_alive_timer(u64 tnow, int interval, u64 iid, bool& finish)
{
	if (!is_registed() || !has_eureka_masternode())
		return;

	if (svrtype_ == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		PRO::Svr_GateSlotUpdate_ntf *ntf = new PRO::Svr_GateSlotUpdate_ntf();
		ntf->set_iid(service_iid_);
		ntf->set_frees(app_proxy_->get_gate_freeslot());

		master_link_->send_to_eureka( ntf);
	}
	else
	{
		PRO::Svr_LiveTick_ntf* ntf = new PRO::Svr_LiveTick_ntf();

		master_link_->send_to_eureka(ntf);
	}
}
