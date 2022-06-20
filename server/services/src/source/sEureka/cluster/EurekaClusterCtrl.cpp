#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>

#include "sEurekaApp.h"
#include "debugdefine.h"

USED_REDISKEY_GLOBAL_NS

EurekaClusterCtrl::EurekaClusterCtrl():lastupdate_(0)
, is_boosted_(false)
{

}

EurekaClusterCtrl::~EurekaClusterCtrl()
{
	release();
}

void EurekaClusterCtrl::release()
{
	link_nodes_.clear();

	wait_links_to_.clear();
	auth_links_to_.clear();
	for (std::list<EurekaLinkTo*>::iterator iter = links_to_.begin(); iter != links_to_.end(); ++iter) {
		delete (*iter);
	}
	links_to_.clear();

	free_links_from_.clear();
	for (std::list<EurekaLinkFrom*>::iterator iter = links_from_.begin(); iter != links_from_.end(); ++iter) {
		delete (*iter);
	}
	links_from_.clear();

	for (boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator iter = redis_nodes.begin(); iter != redis_nodes.end(); ++iter) {
		delete iter->second;
	}
	redis_nodes.clear();
}

bool EurekaClusterCtrl::is_eureka_exist(S_INT_64 iid)
{
	EUREKALINKNODE_MAP::iterator fiter = link_nodes_.find(iid);
	return fiter != link_nodes_.end();
}

bool EurekaClusterCtrl::is_legality_eureka(S_INT_64 iid, S_INT_64 token)
{
	boost::unordered_map<S_INT_64, EurekaNodeInfo*>::iterator fiter = redis_nodes.find(iid);
	if (fiter == redis_nodes.end())
		return false;
	EurekaNodeInfo* ni = fiter->second;
	return ni->is_same_node(token);
}

void EurekaClusterCtrl::on_cantconnect_with_linkto(EurekaLinkTo* plink)
{
	S_INT_64 nodeiid = plink->get_iid();

	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		link_nodes_.erase(nodeiid);
		auth_links_to_.erase(plink);

		//从认证队列删除，放入等待重连队列
		if (is_legality_eureka( nodeiid, plink->get_token()))
		{
			wait_links_to_.insert(plink);
		}
		else
		{
			//节点回收
			wait_links_to_.erase(plink);
		}

		//重置
		plink->force_close();

#ifdef EUREKA_DEBUGINFO_ENABLE
		logDebug(out_sys, "me(eureka) cant connect to eureka node. online nodes:%d", link_nodes_.size());
#endif
	}

	//连接错误警告
	this->warning_eureka_cantconnect(nodeiid);
}

void EurekaClusterCtrl::on_disconnected_with_linkto(EurekaLinkTo* plink)
{
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//断开映射关系
		link_nodes_.erase(plink->get_iid());
		auth_links_to_.erase(plink);

		//有效节点
		if (is_legality_eureka(plink->get_iid(), plink->get_token()))
		{
			//放入等待重新连接队列
			wait_links_to_.insert(plink);
		}
		else
		{
			//节点回收
			wait_links_to_.erase(plink);
		}

		plink->force_close();

#ifdef EUREKA_DEBUGINFO_ENABLE
		logDebug(out_sys, "me(eureka) disconnect with linkto eureka node:%lld. online nodes:%d", plink->get_iid(), link_nodes_.size());
#endif

	}
}

void EurekaClusterCtrl::on_authed_with_linkto(EurekaLinkTo* plink)
{
	S_INT_64 nodeiid = plink->get_iid();

	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//从认证队列删除
		auth_links_to_.erase(plink);

		//IEurekaNodeLink* pp = dynamic_cast<IEurekaNodeLink*>(plink);
		link_nodes_[nodeiid] = plink;
	}
}

void EurekaClusterCtrl::on_disconnected_with_linkfrom(EurekaLinkFrom* plink)
{
	EurekaSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//断开映射关系
		link_nodes_.erase(plink->get_iid());

#ifdef EUREKA_DEBUGINFO_ENABLE
		logDebug(out_sys, "me(eureka) disconnect with linkfrom eureka node:%lld. online nodes:%d", plink->get_iid(), link_nodes_.size());
#endif

		plink->reset();
		psession->reset();

		svrApp.return_freesession_no_mutext(psession);
	}

}
