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

#include "services/ServiceRegisterCtrl.h"

#include <gameLib/config/ConfigHelper.h>

#include "sEurekaApp.h"

ServiceRegisterCtrl::ServiceRegisterCtrl():last_serviceiid_seed_(0)
{
}

ServiceRegisterCtrl::~ServiceRegisterCtrl()
{
}

bool ServiceRegisterCtrl::init_ctrl()
{
	//regist message function
	this->InitNetMessage();

	service_mth_links_.init_holder();

	return true;
}

void ServiceRegisterCtrl::uninit_ctrl()
{
	//release memory of redis cache
	service_mth_meta_release_all();

	service_mth_links_.uninit_holder();
}

void ServiceRegisterCtrl::service_mth_meta_release_all()
{
	router_of_subscribe_.clear();
	service_of_subscribe_.clear();
	servie_of_type_.clear();
	all_service_nodes_.clear();
}

void ServiceRegisterCtrl::service_mth_meta_merge_oftype(NETSERVICE_TYPE type,
	boost::unordered_map<std::string, ServiceNodeInfo*>& sni, std::set<S_INT_64>& delsvrs)
{
	std::list<ServiceNodeInfo*>& dlist = get_service_node_oftype(type);

	std::list<ServiceNodeInfo*> dels;
	for (std::list<ServiceNodeInfo*>::iterator iter2 = dlist.begin(); iter2 != dlist.end(); ++iter2)
	{
		ServiceNodeInfo* si = (*iter2);
		std::string iid = std::to_string(si->iid);
		boost::unordered_map<std::string, ServiceNodeInfo*>::iterator fiter = sni.find(iid.c_str());
		if (fiter == sni.end())
		{
			//need to delete
			dels.push_back(si);
		}
		else
		{
			//重复项保留原来的
			ServiceNodeInfo* pp = fiter->second;
			sni.erase(fiter);
			delete pp;
		}
	}

	//删除多余的服务
	for (std::list<ServiceNodeInfo*>::iterator iter = dels.begin(); iter != dels.end(); ++iter)
	{
		ServiceNodeInfo* p = (*iter);
		delsvrs.insert(p->iid);

		std::list<ServiceNodeInfo*>::iterator fiter = std::find(dlist.begin(), dlist.end(), p);
		if( fiter != dlist.end())
			dlist.erase(fiter);
		delete p;
	}

	//加入新的
	for (boost::unordered_map<std::string, ServiceNodeInfo*>::iterator iter = sni.begin(); iter != sni.end(); ++iter)
	{
		ServiceNodeInfo* p = iter->second;
		dlist.push_back(p);
	}
	sni.clear();
}

std::list<ServiceNodeInfo*> ServiceRegisterCtrl::get_service_node_oftype(NETSERVICE_TYPE type)
{
	boost::unordered_map< NETSERVICE_TYPE, std::list<ServiceNodeInfo*>>::iterator fiter = servie_of_type_.find(type);
	if (fiter == servie_of_type_.end())
	{
		servie_of_type_[type] = std::list<ServiceNodeInfo *>();
		fiter = servie_of_type_.find(type);
	}

	return fiter->second;
}

void ServiceRegisterCtrl::on_mth_disconnected_with_service(ServiceLinkFrom* plink)
{
	EurekaSession* psession = plink->get_session();
	if (psession == 0)
		return;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//断开映射关系
		service_mth_links_.return_freelink(plink);

		plink->reset();
		psession->reset();

		svrApp.return_freesession_no_mutext(psession);
	}
}

ServiceNodeInfo* ServiceRegisterCtrl::find_servicenode_byiid(S_INT_64 iid)
{
	boost::unordered_map<S_INT_64, ServiceNodeInfo>::iterator fiter = all_service_nodes_.find(iid);
	if (fiter == all_service_nodes_.end())
		return 0;
	return &(fiter->second);
}

void ServiceRegisterCtrl::regist_one_service(ServiceNodeInfo& info)
{

}