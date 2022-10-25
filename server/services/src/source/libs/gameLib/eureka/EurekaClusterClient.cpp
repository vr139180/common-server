#include "gameLib/eureka/EurekaClusterClient.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/system/CommandBase.h>
#include <gameLib/LogExt.h>
#include "gameLib/protobuf/Proto_all.h"

EurekaClusterClient& EurekaClusterClient::instance()
{
	static EurekaClusterClient s_eureka;
	return s_eureka;
}

EurekaClusterClient::EurekaClusterClient():
service_iid_(0)
, token_(0)
, cur_link_index_(0)
, app_proxy_(0)
, cur_state_( EurekaState::Eureka_WaitInit)
{
}

EurekaClusterClient::~EurekaClusterClient()
{
}

void EurekaClusterClient::release()
{
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
	, const char* eurekaip, int eurekaport
	, std::list< NETSERVICE_TYPE>& subscribe_service)
{
	this->cur_state_ = EurekaState::Eureka_WaitInit;

	this->app_proxy_ = app;
	this->svrtype_ = type;
	this->myip_ = myip;
	this->myport_ = myport;
	this->extpms_ = extpms;
	this->subscribe_services_ = subscribe_service;

	for (std::list< NETSERVICE_TYPE>::iterator iter = subscribe_services_.begin(); iter != subscribe_services_.end(); ++iter)
	{
		service_nodes_[(*iter)] = SERVICENODE_TYPE();
	}

	//初始化消息处理映射
	this->InitNetMessage();

	EurekaNodeInfo* pnode = new EurekaNodeInfo();
	pnode->ip = eurekaip;
	pnode->port = eurekaport;
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
	size_t num = online_links_.size();
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

	//订阅请求
	//发送订阅请求
	if (is_registed() && online_links_.size() > 0)
	{
		//eureka节点同步
		{
			PRO::Erk_Eureka_sync *syn = new PRO::Erk_Eureka_sync();
			syn->set_myiid(this->get_myiid());

			//缓存的节点信息
			for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = eureka_nodes_.begin(); iter != eureka_nodes_.end(); ++iter)
			{
				syn->add_exists(iter->first);
			}

			this->send_mth_protocol(syn);
		}

		//服务订阅
		{
			PRO::Erk_ServiceSubscribe_req* req = new PRO::Erk_ServiceSubscribe_req();
			req->set_myiid(get_myiid());

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

			this->send_mth_protocol(req, false);
		}
	}
}

bool EurekaClusterClient::is_eurekanode_exist(S_INT_64 sid)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator fiter = eureka_nodes_.find(sid);
	return fiter != eureka_nodes_.end();
}

bool EurekaClusterClient::is_eurekanode_connected(S_INT_64 sid)
{
	for (int ii = 0; ii < online_links_.size(); ++ii)
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
		EurekaNodeInfo* pinfo = plink->clone_node();
		eureka_nodes_[pinfo->iid] = pinfo;
	}

	//连接成功
	this->cur_state_ = EurekaState::Eureka_Registed;
}

void EurekaClusterClient::on_link_bind_result( EurekaClusterLink* plink)
{
	auth_links_.erase(plink);

	//放入有效节点
	std::vector<EurekaClusterLink*>::iterator fiter = std::find(online_links_.begin(), online_links_.end(), plink);
	if (fiter == online_links_.end())
		online_links_.push_back(plink);
}
