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
#include <cmsLib/system/TimerContainer.h>

#include <gameLib/eureka/EurekaNodeInfo.h>

#include "network/EurekaLinkToHolder.h"
#include "network/EurekaLinkFromHolder.h"
#include "network/EurekaLinkTo.h"
#include "network/EurekaLinkFrom.h"

#include "cluster/EurekaLostMaintance.h"

class sEurekaApp;

/**
* eureka��cluster�߼�ά��
* clusterctrl�����к������������̵߳ĵ��̻߳�����ִ�е�
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
	//�Ƿ���master�ڵ�
	bool is_master() { return myself_.ismaster && myself_.iid == eureka_master_iid_; }
	bool check_node_is_master(S_INT_64 nodeid);

	//�ڵ��Ƿ��Ѿ�ע��
	bool is_eurekalink_exist(S_INT_64 iid);
	//���ӵĽڵ���
	S_INT_32 get_lived_eurekanode_size();
	bool is_eureka_node(S_INT_64 iid);
	EurekaNodeInfo* get_eureka_node(S_INT_64 iid);
	//�Ƿ�����Ч�Ľڵ�
	bool is_eureka_node(S_INT_64 iid, S_INT_64 token);
	S_INT_32 eureka_node_size() { return (S_INT_32)eureka_nodes_.size(); }

	//msg��Ҫ�����߹������û����ͷ�
	void broadcast_to_eurekas( BasicProtocol* msg, IEurekaNodeLink* ignore = 0);

	//ǿ������
	bool force_reconnect( S_INT_64 iid);

	void sync_slaver_eurekaseed(S_INT_64 iid) { last_eureka_iid_ = iid; }
	S_INT_64 get_eureka_seed() { return last_eureka_iid_; }


protected:
	void release();

	//------------------------------master���-------------------------
	//master������������
	S_INT_64 make_next_eurekaiid();
	S_INT_64 make_next_eurekaiid_fix(S_INT_32 incstep = 10000) { last_eureka_iid_ += incstep;  return ++last_eureka_iid_; }

	//��redis����eureka�ڵ�
	bool update_redis_masterinfo(RedisClient* rdv);
	//ʹ�Լ���Ϊmaster
	void make_me_masternode();
	void master_regist_one_slaver(EurekaNodeInfo& node);
	void master_invalid_one_slaver(S_INT_64 nodiid);

	//ͬ��ȫ����Ϣ��slaver
	void master_sync_all_to_slaver( EurekaLinkFrom* from);
	//֪ͨ�ڵ���
	void master_notify_change_to_slaver(EurekaLinkFrom* ignore);

	//------------------------------slaver���-------------------------
	//ע�ᵽmaster
	void slaver_regist_to_master(EurekaNodeInfo info);
	void slaver_sync_eurekanodes(PRO::Erk_EurekaUpdate_ntf* ntf);
	//�ڵ�ʧЧ
	void slaver_eurekanode_invalid(S_INT_64 nodiid);
	//��ѡmaster
	void slaver_vote_master();
	void slaver_try_vote_master();
	
	//------------------------------commons---------------------------
	//����һ���ڵ㶪ʧ����
	void fire_common_eurekanode_lost(S_INT_64 nodiid);
	EurekaLostMaintance* get_eurekanode_lost(S_INT_64 nodiid);
	void tellme_eurekanode_lost( S_INT_64 fromiid, S_INT_64 lostnode);
	//���ڵ� 0: waiting 1:lost 2:well
	S_INT_32 check_eurekanode_lost_well(S_INT_64 lostnode);
	//ע��ɹ�֪ͨ
	void someone_eurekanode_authed(S_INT_64 nodiid);
	//����ڵ㶪ʧ
	void do_eurekanode_lost(S_INT_64 lostnode);

protected:
	void eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish);

	void vote_master_timer(u64 tnow, int interval, u64 iid, bool& finish);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(NetProtocol* message, bool& autorelease){}

public:
	//protocal process function
	void on_eurekaregist_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_eurekaregist_ack(NetProtocol* pro, bool& autorelease, EurekaLinkTo* plinkto);

	void on_eurekabind_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_eurekabind_ack(NetProtocol* pro, bool& autorelease, EurekaLinkTo* plinkto);

	void on_eurekaupdate_ntf(NetProtocol* pro, bool& autorelease);
	void on_masterchange_ntf(NetProtocol* pro, bool& autorelease);
	void on_servicesync_ntf(NetProtocol* pro, bool& autorelease);
	void on_serviceshutdown_ntf(NetProtocol* pro, bool& autorelease);
	void on_eurekalost_ntf(NetProtocol* pro, bool& autorelease);

	//---------------------------system command--------------------------------
	void on_disconnected_with_linkto(EurekaLinkTo* plink);
	void on_linkto_regist_result(EurekaLinkTo* plink);

	void on_disconnected_with_linkfrom(EurekaLinkFrom* plink);

private:
	EurekaNodeInfo		myself_;
	//ע��ɹ�֮����ܿ�ʼ������
	bool				is_boosted_;
	S_INT_64			eureka_master_iid_;
	//����iid������
	S_INT_64			last_eureka_iid_;
	//redis��ע���eureka�ڵ���Ϣ
	boost::unordered_map<S_INT_64, EurekaNodeInfo> eureka_nodes_;

	EurekaLinkFromHolder<EurekaLinkFrom>	eureka_links_from_;
	EurekaLinkToHolder<EurekaLinkTo>		eureka_links_to_;

	//���߼������
	boost::unordered_map<S_INT_64, EurekaLostMaintance*>	lost_maintance_;
	//��ѡmaster,������
	S_INT_32 vote_master_retry_;
	TimerKey vote_key_;
};

#endif //__EUREKACLUSTERCTRL_H__