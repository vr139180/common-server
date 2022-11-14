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

#include "DataRouterApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void DataRouterApp::on_mth_servicebindservice_req(NetProtocol* pro, bool& autorelease, void* session)
{
	Svr_ServiceBindService_req* req = dynamic_cast<Svr_ServiceBindService_req*>(pro->msg_);
	RouterSession* psession = reinterpret_cast<RouterSession*>(session);

	//service没有变化
	if (req->toiid() != EurekaClusterClient::instance().get_myiid()
		|| req->totoken() != EurekaClusterClient::instance().get_token())
	{
		psession->force_close();
		return;
	}

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
	{
		psession->force_close();
		return;
	}

	Svr_ServiceBindService_ack* ack = new Svr_ServiceBindService_ack();
	std::unique_ptr<Svr_ServiceBindService_ack> ptr(ack);

	ack->set_result(1);
	ack->set_svr_type(req->svr_type());
	ack->set_toiid(req->toiid());
	ack->set_totoken(req->totoken());

	//来自gate的注册
	if (ctype == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		GateServiceLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = gate_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			gate_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER)
	{
		ServiceRouterLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = servicerouter_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			servicerouter_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER)
	{
		FightRouterLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = fightrouter_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			fightrouter_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_HOME)
	{
		HomeServiceLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = home_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			home_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_STATE)
	{
		StateServiceLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = state_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			state_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_LOGIN)
	{
		LoginServiceLinkFrom *pLink = 0;
		{
			ThreadLockWrapper guard(get_threadlock());

			session_from_.remove_waitsession_mth(psession);
			pLink = login_links_from_.ask_free_link();

			pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

			psession->auth();
			pLink->set_session(psession);
			psession->set_netlinkbase(pLink);

			//设置当前gatelinke
			login_links_from_.regist_onlinelink(pLink);

			pLink->registinfo_tolog(true);
		}

		ack->set_result(0);
		pLink->send_netprotocol(ptr.release());
	}
}

//-------------------------------------------------------eureka cluster---------------------------------------
void DataRouterApp::mth_notify_servicenode_new(NETSERVICE_TYPE,
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
}

void DataRouterApp::mth_notify_routerbalance_new(NETSERVICE_TYPE, std::list<S_INT_64>& svrs)
{

}

void DataRouterApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< datarouter node:%lld online to eureka >>>>>>>>>>>>", sid);
	
	this->is_ready_ = true;

	//sid作为custom channel id的种子
}

void DataRouterApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "datarouter[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}