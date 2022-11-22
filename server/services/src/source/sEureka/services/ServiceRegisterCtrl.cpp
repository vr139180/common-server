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

std::list<ServiceNodeInfo*>& ServiceRegisterCtrl::get_service_node_oftype(NETSERVICE_TYPE type)
{
	boost::unordered_map< NETSERVICE_TYPE, std::list<ServiceNodeInfo*>>::iterator fiter = servie_of_type_.find(type);
	if (fiter == servie_of_type_.end())
	{
		servie_of_type_[type] = std::list<ServiceNodeInfo *>();
		fiter = servie_of_type_.find(type);
	}

	return fiter->second;
}

std::list<S_INT_64>& ServiceRegisterCtrl::get_subscribes_oftype(NETSERVICE_TYPE type)
{
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>>::iterator fiter = service_of_subscribe_.find(type);
	if (fiter == service_of_subscribe_.end())
	{
		service_of_subscribe_[type] = std::list<S_INT_64>();
		fiter = service_of_subscribe_.find(type);
	}

	return fiter->second;
}

std::list<S_INT_64>& ServiceRegisterCtrl::get_routers_oftype(NETSERVICE_TYPE type)
{
	boost::unordered_map<NETSERVICE_TYPE, std::list<S_INT_64>>::iterator fiter = router_of_subscribe_.find(type);
	if (fiter == router_of_subscribe_.end())
	{
		router_of_subscribe_[type] = std::list<S_INT_64>();
		fiter = router_of_subscribe_.find(type);
	}

	return fiter->second;
}

void ServiceRegisterCtrl::add_service_to_subscribe(NETSERVICE_TYPE type, S_INT_64 svriid)
{
	std::list<S_INT_64>& subs = get_subscribes_oftype(type);
	if (std::find(subs.begin(), subs.end(), svriid) == subs.end())
		subs.push_back( svriid);
}

void ServiceRegisterCtrl::add_service_to_router(NETSERVICE_TYPE type, S_INT_64 svriid)
{
	std::list<S_INT_64>& subs = get_routers_oftype(type);
	if (std::find(subs.begin(), subs.end(), svriid) == subs.end())
		subs.push_back(svriid);
}

void ServiceRegisterCtrl::del_service_from_subscribe(NETSERVICE_TYPE type, S_INT_64 svriid)
{
	std::list<S_INT_64>& subs = get_subscribes_oftype(type);
	std::list<S_INT_64>::iterator fiter = std::find(subs.begin(), subs.end(), svriid);
	if ( fiter != subs.end())
		subs.erase(fiter);
}

void ServiceRegisterCtrl::del_service_from_router(NETSERVICE_TYPE type, S_INT_64 svriid)
{
	std::list<S_INT_64>& subs = get_routers_oftype(type);
	std::list<S_INT_64>::iterator fiter = std::find(subs.begin(), subs.end(), svriid);
	if (fiter != subs.end())
		subs.erase(fiter);
}

void ServiceRegisterCtrl::offline_one_service(S_INT_64 iid)
{
	ServiceNodeInfo* pnod = find_servicenode_byiid(iid);
	if (pnod == 0)
		return;

	for (std::list<NETSERVICE_TYPE>::iterator iter = pnod->subscribes_.begin(); iter != pnod->subscribes_.end(); ++iter)
	{
		NETSERVICE_TYPE ctype = (*iter);
		del_service_from_subscribe(ctype, iid);
	}

	for (std::list<NETSERVICE_TYPE>::iterator iter = pnod->routers_.begin(); iter != pnod->routers_.end(); ++iter)
	{
		NETSERVICE_TYPE ctype = (*iter);
		del_service_from_router(ctype, iid);
	}

	std::list<ServiceNodeInfo*>& lls = get_service_node_oftype(pnod->type);
	std::list<ServiceNodeInfo*>::iterator fiter = std::find(lls.begin(), lls.end(), pnod);
	if (fiter != lls.end())
		lls.erase(fiter);

	all_service_nodes_.erase(iid);
}

void ServiceRegisterCtrl::on_mth_disconnected_with_service(ServiceLinkFrom* plink)
{
	EurekaSession* psession = plink->get_session();
	if (psession == 0)
		return;

	S_INT_64 svriid = plink->get_iid();
	NETSERVICE_TYPE ctype = plink->get_node()->type;

	plink->registinfo_tolog(false);

	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		//断开映射关系
		service_mth_links_.return_freelink(plink);

		plink->reset();
		psession->reset();

		svrApp.return_freesession_no_mutext(psession);
	}

	//master检测到断开，直接踢出服务
	if (svrApp.get_eurekactrl()->is_master())
	{
		offline_one_service(svriid);

		//sync to slaver nodes
		Erk_ServiceSync_ntf* ntf = new Erk_ServiceSync_ntf();
		std::unique_ptr< Erk_ServiceSync_ntf> ptr(ntf);

		ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
		ntf->set_fullsvrs(false);
		ntf->add_offline(svriid);
		ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
		ntf->set_service_seed(this->get_serviceiid_seed());

		svrApp.get_eurekactrl()->broadcast_to_eurekas(ntf);

		//notify all service
		notify_service_offline(svriid, ctype);
	}
}

S_INT_64 ServiceRegisterCtrl::make_next_serviceiid()
{
	S_INT_64 iid = ++last_serviceiid_seed_;
	if (find_servicenode_byiid(iid) != 0)
		iid = make_next_serviceiid_fix();

	return iid;
}

ServiceNodeInfo* ServiceRegisterCtrl::find_servicenode_byiid(S_INT_64 iid)
{
	boost::unordered_map<S_INT_64, ServiceNodeInfo>::iterator fiter = all_service_nodes_.find(iid);
	if (fiter == all_service_nodes_.end())
		return 0;
	return &(fiter->second);
}

ServiceNodeInfo* ServiceRegisterCtrl::regist_one_service(ServiceNodeInfo& info, bool forceupdate)
{
	ServiceNodeInfo* pnode = find_servicenode_byiid(info.iid);
	if (pnode != 0)
	{
		if (forceupdate)
			*pnode = info;

		return pnode;
	}

	all_service_nodes_[info.iid] = info;
	pnode = find_servicenode_byiid(info.iid);

	//加入其他关联
	std::list<ServiceNodeInfo*>& lls = get_service_node_oftype((NETSERVICE_TYPE)info.type);
	lls.push_back(pnode);

	return pnode;
}

BasicProtocol* ServiceRegisterCtrl::master_syncall_servicenodes()
{
	Erk_ServiceSync_ntf * ntf = new Erk_ServiceSync_ntf();
	ntf->set_masteriid(svrApp.get_eurekactrl()->get_myself().iid);
	ntf->set_fullsvrs(true);
	ntf->set_eureka_seed(svrApp.get_eurekactrl()->get_eureka_seed());
	ntf->set_service_seed(this->get_serviceiid_seed());

	for (boost::unordered_map<S_INT_64, ServiceNodeInfo>::iterator iter = all_service_nodes_.begin(); iter != all_service_nodes_.end(); ++iter)
	{
		ServiceNodeInfo* pnode = &(iter->second);
		PRO::ServerNode* pnew = ntf->add_newsvrs();
		pnode->copy_to(pnew);
	}

	return ntf;
}

void ServiceRegisterCtrl::broadcast_to_allsvrs(BasicProtocol* msg)
{
	service_mth_links_.broadcast(msg);
}

void ServiceRegisterCtrl::notify_service_online(S_INT_64 iid)
{
	ServiceNodeInfo* pNew = find_servicenode_byiid(iid);
	if (pNew == 0)
		return;

	NETSERVICE_TYPE ctype = pNew->type;
	//subscribes notify
	std::list<S_INT_64>& subs = get_subscribes_oftype(ctype);
	if (subs.size() > 0)
	{
		for (std::list<S_INT_64>::iterator iter = subs.begin(); iter != subs.end(); ++iter)
		{
			S_INT_64 tiid = (*iter);
			ServiceLinkFrom* pfrom = service_mth_links_.get_servicelink_byiid(tiid);
			if (pfrom == 0)
				continue;

			Erk_ServiceSubscribe_ntf* ntf = new Erk_ServiceSubscribe_ntf();
			ntf->set_myiid(pfrom->get_iid());
			ntf->set_svr_type(ctype);
			PRO::ServerNode* psvr = ntf->add_newsvrs();
			pNew->copy_to(psvr);

			pfrom->send_to_service(ntf);
		}
	}

	//router notify
	subs = get_routers_oftype(ctype);
	if (subs.size() > 0)
	{
		std::list<ServiceNodeInfo*>& svrs = get_service_node_oftype(ctype);
		if (svrs.size() > 0)
		{
			for (std::list<S_INT_64>::iterator iter = subs.begin(); iter != subs.end(); ++iter)
			{
				S_INT_64 tiid = (*iter);
				ServiceLinkFrom* pfrom = service_mth_links_.get_servicelink_byiid(tiid);
				if (pfrom == 0)
					continue;

				Erk_RouterSubscribe_ntf* ntf = new Erk_RouterSubscribe_ntf();
				ntf->set_myiid(pfrom->get_iid());
				ntf->set_svr_type(ctype);

				for (std::list<ServiceNodeInfo*>::iterator iter = svrs.begin(); iter != svrs.end(); ++iter)
				{
					ServiceNodeInfo* pinf = (*iter);
					ntf->add_svriids( pinf->iid);
				}

				pfrom->send_to_service(ntf);
			}
		}
	}
}

void ServiceRegisterCtrl::notify_service_offline(S_INT_64 iid, NETSERVICE_TYPE ctype)
{
	//subscribes notify
	std::list<S_INT_64>& subs = get_subscribes_oftype(ctype);
	if (subs.size() > 0)
	{
		for (std::list<S_INT_64>::iterator iter = subs.begin(); iter != subs.end(); ++iter)
		{
			S_INT_64 tiid = (*iter);
			ServiceLinkFrom* pfrom = service_mth_links_.get_servicelink_byiid(tiid);
			if (pfrom == 0)
				continue;

			Erk_ServiceSubscribe_ntf* ntf = new Erk_ServiceSubscribe_ntf();
			ntf->set_myiid(pfrom->get_iid());
			ntf->set_svr_type(ctype);
			ntf->add_offsvrs(iid);

			pfrom->send_to_service(ntf);
		}
	}

	//router notify
	subs = get_routers_oftype(ctype);
	if (subs.size() > 0)
	{
		std::list<ServiceNodeInfo*>& svrs = get_service_node_oftype(ctype);
		for (std::list<S_INT_64>::iterator iter = subs.begin(); iter != subs.end(); ++iter)
		{
			S_INT_64 tiid = (*iter);
			ServiceLinkFrom* pfrom = service_mth_links_.get_servicelink_byiid(tiid);
			if (pfrom == 0)
				continue;

			Erk_RouterSubscribe_ntf* ntf = new Erk_RouterSubscribe_ntf();
			ntf->set_myiid(pfrom->get_iid());
			ntf->set_svr_type(ctype);

			for (std::list<ServiceNodeInfo*>::iterator iter = svrs.begin(); iter != svrs.end(); ++iter)
			{
				ServiceNodeInfo* pinf = (*iter);
				ntf->add_svriids(pinf->iid);
			}

			pfrom->send_to_service(ntf);
		}
	}
}
