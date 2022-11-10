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

#include "cluster/EurekaClusterCtrl.h"

#include <cmsLib/base/OSSystem.h>
#include <cmsLib/json/JSONUtil.h>
#include <gameLib/redis/global_redis_const.h>
#include <gameLib/config/ConfigHelper.h>
#include <gameLib/protobuf/Proto_all.h>

#include "sEurekaApp.h"

USE_PROTOCOL_NAMESPACE
USED_REDISKEY_GLOBAL_NS

void EurekaClusterCtrl::InitNetMessage()
{
	REGISTERMSG(ERK_PROTYPE::ERK_EUREKAUPDATE_NTF, &EurekaClusterCtrl::on_eurekaupdate_ntf, this);
	REGISTERMSG(ERK_PROTYPE::ERK_MASTERCHANGE_NTF, &EurekaClusterCtrl::on_masterchange_ntf, this);
}

void EurekaClusterCtrl::on_eurekaregist_req(NetProtocol* pro, bool& autorelease, void* session)
{
	EurekaSession* pes = reinterpret_cast<EurekaSession*>(session);
	Erk_EurekaRegist_req* req = reinterpret_cast<Erk_EurekaRegist_req*>(pro->msg_);
}

void EurekaClusterCtrl::on_eurekabind_req(NetProtocol* pro, bool& autorelease, void* session)
{
	EurekaSession* pes = reinterpret_cast<EurekaSession*>(session);
	Erk_EurekaBind_req* req = reinterpret_cast<Erk_EurekaBind_req*>(pro->msg_);

	if (is_eureka_exist(req->iid()) || (!is_boosted()))
	{
		//强制挂断
		pes->force_close();

		logError(out_runtime, "one eureka:%lld bind request failed. already binded.", req->iid());
		return;
	}
	
	//建立关联
	EurekaLinkFrom* pfrom = 0;
	{
		ThreadLockWrapper guard(svrApp.get_threadlock());

		svrApp.remove_waitsession_no_mutex(pes);
		pfrom = eureka_links_from_.ask_free_link();

		pfrom->set_linkbase_info(req->iid(), req->token());

		pes->auth();
		pfrom->set_session(pes);
		pes->set_netlinkbase(pfrom);

		//设置当前gatelinke
		eureka_links_from_.regist_onlinelink(pfrom);

		pfrom->registinfo_tolog(true);

		/*
		EurekaNodeInfo* pnode = new EurekaNodeInfo();
		pnode->iid = req->iid();
		pnode->token = req->token();
		pnode->ip = req->ip().c_str();
		pnode->port = req->port();

		//关联node
		pfrom->set_node(*pnode);
		*/
	}

	if (pfrom)
	{
		Erk_EurekaBind_ack *ack = new Erk_EurekaBind_ack();
		ack->set_result(0);
		pfrom->send_to_eureka(ack);
	}
}

void EurekaClusterCtrl::on_eurekaupdate_ntf(NetProtocol* pro, bool& autorelease)
{

}

void EurekaClusterCtrl::on_masterchange_ntf(NetProtocol* pro, bool& autorelease)
{

}
