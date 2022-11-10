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

#ifndef __EUREKACLUSTERCTRL_H__
#define __EUREKACLUSTERCTRL_H__

#include <boost/unordered_map.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/ThreadLock.h>
#include <cmsLib/CommandListener.h>

#include <gameLib/eureka/EurekaNodeInfo.h>

#include "network/EurekaLinkToHolder.h"
#include "network/EurekaLinkFromHolder.h"
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

public:
	EurekaClusterCtrl();
	~EurekaClusterCtrl();

	void init_ctrl();
	void unint_ctrl();

	bool boot_ctrl();
	void init_timer();

public:
	bool is_boosted() { return this->is_boosted_; }
	void mark_boosted() { this->is_boosted_ = true; }
	EurekaNodeInfo& get_myself() { return myself_; }
	//是否是master节点
	bool is_master() { return myself_.ismaster && myself_.iid == eureka_master_iid_; }
	bool check_node_is_master(S_INT_64 nodeid);

	//节点是否已经注册
	bool is_eureka_exist(S_INT_64 iid);
	//是否是有效的节点
	bool is_legality_eureka(S_INT_64 iid, S_INT_64 token);

	//master管理服务器编号
	S_INT_32 make_next_eurekaiid() { return ++last_eureka_iid_; }

protected:
	void release();

	//从redis更新eureka节点
	bool update_redis_masterinfo(RedisClient* rdv);

	//注册到master
	void try_regist_to_master(EurekaNodeInfo info);

protected:
	void eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease){}

public:
	//protocal process function
	void on_eurekaregist_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_eurekabind_req(NetProtocol* pro, bool& autorelease, void* session);

	void on_eurekaupdate_ntf(NetProtocol* pro, bool& autorelease);
	void on_masterchange_ntf(NetProtocol* pro, bool& autorelease);

	//---------------------------system command--------------------------------
	void on_disconnected_with_linkto(EurekaLinkTo* plink);
	void on_linkto_regist_result(EurekaLinkTo* plink);

	void on_disconnected_with_linkfrom(EurekaLinkFrom* plink);

private:
	EurekaNodeInfo		myself_;
	//注册成功之后才能开始后续的
	bool				is_boosted_;
	S_INT_64			eureka_master_iid_;

	S_INT_64			lastupdate_;

	//redis中注册的eureka节点信息
	boost::unordered_map<S_INT_64, EurekaNodeInfo> eureka_nodes_;

	EurekaLinkFromHolder<EurekaLinkFrom>	eureka_links_from_;
	EurekaLinkToHolder<EurekaLinkTo>		eureka_links_to_;

	//服务iid生成器
	S_INT_32			last_eureka_iid_;
};

#endif //__EUREKACLUSTERCTRL_H__