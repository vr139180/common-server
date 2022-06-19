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
* eureka��cluster�߼�ά��
* clusterctrl�����к������������̵߳ĵ��̻߳�����ִ�е�
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
	//�ڵ��Ƿ��Ѿ�ע��
	bool is_eureka_exist(S_INT_64 iid);
	//�Ƿ�����Ч�Ľڵ�
	bool is_legality_eureka(S_INT_64 iid, S_INT_64 token);

protected:
	void release();

	//��redis����eureka�ڵ�
	void update_redis_eurekas();

protected:
	//ע��󶨾���
	void warning_eureka_cantconnect(S_INT_64 toiid);
	void warning_eurekabind_failed(S_INT_64 fromiid, S_INT_64 toiid);
	void warning_eureka_disconnect(S_INT_64 fromiid, S_INT_64 toiid);

	//�ڵ�ά������redis��eureka�ڵ���Ϣ�͵�ǰ��������Ϣ���й���
	void maintnce_eurekanode_allcheck(S_TIMESTAMP tnow);
	//ͬ�����õ�eureka�ڵ�,�����Լ�
	void maintnce_sync_alleureka();

protected:
	void eureka_auto_connect_timer(u64 tnow, int interval, u64 iid, bool& finish);
	//��ʱ�����������ݵ�redis
	//ά����ǰ������
	void eureka_update_heartbeat_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void eureka_maintnce_timer(u64 tnow, int interval, u64 iid, bool& finish);
	//link to �������
	void eureka_linkto_heart_beat(u64 tnow, int interval, u64 iid, bool& finish);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage(BasicProtocol* message, bool& autorelease, int msgid){}

public:
	//protocal process function
	void on_eurekabind_req(BasicProtocol* pro, bool& autorelease, void* session);

	//servie���͵�eureka�ڵ�ͬ������
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

	//redis��ע���eureka�ڵ���Ϣ
	boost::unordered_map<S_INT_64, EurekaNodeInfo*> redis_nodes;

	//���еĿ�������
	EUREKALINKNODE_MAP			link_nodes_;

	//�ȴ����е�����
	std::set<EurekaLinkTo*>		wait_links_to_;
	std::set<EurekaLinkTo*>		auth_links_to_;
	//to ����EurekaLinkTo�����ж��������Ķ��Ƕ���Щ���������
	std::list<EurekaLinkTo*>	links_to_;

	//from
	std::list<EurekaLinkFrom*>	links_from_;
	std::set<EurekaLinkFrom*>	free_links_from_;
};

#endif //__EUREKACLUSTERCTRL_H__