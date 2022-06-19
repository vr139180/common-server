#ifndef __SERVICEREGISTERCTRL_H__
#define __SERVICEREGISTERCTRL_H__

#include <set>
#include <boost/scoped_array.hpp>
#include <boost/unordered_map.hpp>
#include <cmsLib/CommandListener.h>

#include <gameLib/service_type.h>
#include <gameLib/commons/LinkFromHolder.h>

#include "network/ServiceLinkFrom.h"

class ServiceRegisterCtrl : public MessageProcess
{
public:
	ServiceRegisterCtrl();
	virtual ~ServiceRegisterCtrl();

	bool init_ctrl();
	void uninit_ctrl();
	void init_timer();

protected:
	//---------eureka�˻���� �������͵�service������
	void redis_update_mth_serviemeta();

	void service_mth_meta_release_all();
	void service_mth_meta_merge_oftype(NETSERVICE_TYPE type, 
		boost::unordered_map<std::string, ServiceNodeInfo*>& sni, std::set<S_INT_64>& delsvrs);
	std::list<ServiceNodeInfo*>& get_service_meth_meta_oftype(NETSERVICE_TYPE type);

	boost::unordered_map< NETSERVICE_TYPE, std::list<ServiceNodeInfo*>>	servie_of_type_redis_;

protected:
	//���service����,ֻ�����߳���ʹ��,���뱣֤�̰߳�ȫ
	LinkFromHolder<ServiceLinkFrom>		service_mth_links_;

public:
	void on_mth_serviceregist_req(BasicProtocol* pro, bool& autorelease, void* session);
	//ackȷ��֮��ע��ķ�����redis�����б��вŻ�������Ч
	void on_mth_serviceregist_confirm(BasicProtocol* pro, bool& autorelease);
	void on_mth_servicebind_req(BasicProtocol* pro, bool& autorelease, void* session);
	void on_mth_disconnected_with_service(ServiceLinkFrom* plink);
	void on_mth_message_route_to_service(BasicProtocol* pro, bool& autorelease, int sid);

	//��������Ϣͬ��
	void on_mth_servicesubscribe_req(BasicProtocol* pro, bool& autorelease);

	void on_mth_gatebindhome_req(BasicProtocol* pro, bool& autorelease);
	void on_mth_gatebindhome_confirm(BasicProtocol* pro, bool& autorelease);

protected:
	void redis_serviceregist_do1(ServiceNodeInfo* pnode);
	void redis_serviceregist_do2(S_INT_64 sid);

	//gate��home ret����ֵ,
	int redis_gatebindhome_do(S_INT_64 gateid, S_INT_64 gatetoken, S_INT_64 bindtoken, ServiceNodeInfo& node);
	void redis_gatebindhome_confirm(S_INT_64 gateid, S_INT_64 homeid, S_INT_64 bindtoken);
	void redis_gatebindhome_maintnce(S_INT_64 sid, NETSERVICE_TYPE type);

	void maintnce_service(u64 now);
	void maintnce_gatebindhome_authtimeout(u64 now);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage( BasicProtocol* message, bool& autorelease, int msgid) {}

	//----------------------timer----------------------------------------------------
	void service_sync_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void service_maintnce_timer(u64 tnow, int interval, u64 iid, bool& finish);
	void gatebindhome_authtimeout_timer(u64 tnow, int interval, u64 iid, bool& finish);
};


#endif //__SERVICEREGISTERCTRL_H__