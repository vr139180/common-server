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

#ifndef __SERVICEREGISTERCTRL_H__
#define __SERVICEREGISTERCTRL_H__

#include <set>
#include <boost/scoped_array.hpp>
#include <boost/unordered_map.hpp>
#include <cmsLib/CommandListener.h>

#include <gameLib/service_type.h>

#include "network/ServiceLinkFromHolder.h"
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
	S_INT_64 make_next_serviceiid() { return ++last_serviceiid_seed_; }
	S_INT_64 make_next_serviceiid_fix(S_INT_64 incstep = 10000) { last_serviceiid_seed_ += incstep; return ++last_serviceiid_seed_; }

	ServiceNodeInfo* find_servicenode_byiid(S_INT_64 iid);

	//---------eureka�˻���� �������͵�service������
	void service_mth_meta_release_all();

	void service_mth_meta_merge_oftype(NETSERVICE_TYPE type, 
		boost::unordered_map<std::string, ServiceNodeInfo*>& sni, std::set<S_INT_64>& delsvrs);

	std::list<ServiceNodeInfo*> get_service_node_oftype(NETSERVICE_TYPE type);

	void regist_one_service(ServiceNodeInfo& info);

protected:
	//���service����,ֻ�����߳���ʹ��,���뱣֤�̰߳�ȫ
	ServiceLinkFromHolder<ServiceLinkFrom>				service_mth_links_;

	//���еķ���ڵ���Ϣ
	boost::unordered_map<S_INT_64, ServiceNodeInfo>		all_service_nodes_;
	//���������Ϣ
	boost::unordered_map<NETSERVICE_TYPE, std::list<ServiceNodeInfo*>>	servie_of_type_;
	//��������Ϣ key:�������� value:�������б�
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>> service_of_subscribe_;
	//���ؾ��ⶩ�� key:�������� value:�������б�
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>> router_of_subscribe_;

	S_INT_64	last_serviceiid_seed_;

public:
	void on_mth_disconnected_with_service(ServiceLinkFrom* plink);

	//---------------------- MessageProcess interface--------------------------------
	virtual void InitNetMessage();
	virtual void ProcessMessage( NetProtocol* message, bool& autorelease) {}

	void on_mth_serviceregist_req(NetProtocol* pro, bool& autorelease, void* session);
	void on_mth_servicebind_req(NetProtocol* pro, bool& autorelease, void* session);

	//��������Ϣͬ��
	void on_mth_servicesync_ntf(NetProtocol* pro, bool& autorelease);
	void on_mth_servicesubscribe_req(NetProtocol* pro, bool& autorelease);
	void on_mth_servicesubscribe_ntf(NetProtocol* pro, bool& autorelease);

	void on_mth_routersubscribe_req(NetProtocol* pro, bool& autorelease);
	void on_mth_routersubscribe_ntf(NetProtocol* pro, bool& autorelease);
	void on_mth_routeronline_req(NetProtocol* pro, bool& autorelease);
	void on_mth_routeronline_ntf(NetProtocol* pro, bool& autorelease);

	void on_mth_serviceshutdown_ntf(NetProtocol* pro, bool& autorelease);

};


#endif //__SERVICEREGISTERCTRL_H__