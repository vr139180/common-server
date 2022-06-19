#ifndef __EUREKACLUSTERCTRL_H__
#define __EUREKACLUSTERCTRL_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/prolib/core_type.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/CommandListener.h>

#include <gameLib/eureka/EurekaNodeInfo.h>

#include "network/EurekaLinkTo.h"
#include "network/EurekaLinkFrom.h"

class sEurekaApp;

/**
* eureka的cluster逻辑维护
* clusterctrl的所有函数都是在主线程的单线程环境下执行的
*
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2021
*/
class EurekaClusterCtrl : public MessageProcess
{
	friend class sEurekaApp;

	typedef boost::unordered_map<S_INT_64, IEurekaNodeLink*>	EUREKALINKNODE_MAP;
public:
	EurekaClusterCtrl();
	~EurekaClusterCtrl();

	bool init_ctrl();
	void unint_ctrl();
	void init_timer();

public:
	bool is_boosted() { return this->is_boosted_; }
	void mark_boosted() { this->is_boosted_ = true; }
	EurekaNodeInfo& get_myself() { return myself_; }
	//节点是否已经注册
	bool is_eureka_exist(S_INT_64 iid);
	//是否是有效的节点
	bool is_legality_eureka(S_INT_64 iid, S_INT_64 token);

protected:
	void release();

	//从redis更新eureka节点
	void update_redis_eurekas();

protected:
	//注册绑定警告
	void warning_eureka_cantconnect(S_INT_64 toiid);
	void warning_eurekabind_failed(S_INT_64 fromiid, S_INT_64 toiid);
	void warning_eureka_disconnect(S_INT_64 fromiid, S_INT_64 toiid);

	//节点维护根据redis的eureka节点信息和当前的链接信息进行管理
	void maintnce_eurekanode_allcheck(S_TIMESTAMP tnow);
	//同步可用的eureka节点,包含自己
	void maintnce_sync_alleureka();

protected:
	void eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish);
	//定时更新自身数据到redis
	//维护当前的链接
	void eureka_update_heartbeat_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void eureka_maintnce_timer(u64 tnow, int interval, u64 iid, bool& finish);
	//link to 心跳检查
	void eureka_linkto_heart_beat(u64 tnow, int interval, u64 iid, bool& finish);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(BasicProtocol* message, bool& autorelease, int msgid){}

public:
	//protocal process function
	void on_eurekabind_req(BasicProtocol* pro, bool& autorelease, void* session);

	//servie发送的eureka节点同步请求
	void on_service_eureka_sync(BasicProtocol* message, bool& autorelease);

	//---------------------------system command--------------------------------
	void on_disconnected_with_linkto(EurekaLinkTo* plink);
	void on_authed_with_linkto(EurekaLinkTo* plink);
	void on_cantconnect_with_linkto(EurekaLinkTo* plink);

	void on_disconnected_with_linkfrom(EurekaLinkFrom* plink);

private:
	EurekaNodeInfo		myself_;
	S_INT_64			lastupdate_;
	bool				is_boosted_;

	//redis中注册的eureka节点信息
	boost::unordered_map<S_INT_64, EurekaNodeInfo*> redis_nodes;

	//所有的可用链接
	EUREKALINKNODE_MAP			link_nodes_;

	//等待进行的链接
	std::set<EurekaLinkTo*>		wait_links_to_;
	std::set<EurekaLinkTo*>		auth_links_to_;
	//to 保存EurekaLinkTo的所有对象，其他的都是对这些对象的引用
	std::list<EurekaLinkTo*>	links_to_;

	//from
	std::list<EurekaLinkFrom*>	links_from_;
	std::set<EurekaLinkFrom*>	free_links_from_;
};

#endif //__EUREKACLUSTERCTRL_H__