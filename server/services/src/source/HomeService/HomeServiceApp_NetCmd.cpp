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

#include "HomeServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/commons/ConsistentHashChecker.h>

USE_PROTOCOL_NAMESPACE

void HomeServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE type, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
	logDebug(out_runtime, "recv new service type:%s node:%d unvalide node:%d from eureka",
		NetServiceType::to_string(type).c_str(), nodes.size(), deliids.size());

	//sync res service
	if (type == NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER)
	{
		datarouter_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void HomeServiceApp::mth_notify_routerbalance_new(NETSERVICE_TYPE type, std::list<S_INT_64>& svrs)
{
	logDebug(out_runtime, "recv routerbalance sycn type:%s node:%d from eureka",
		NetServiceType::to_string(type).c_str(), svrs.size());

	if (type == NETSERVICE_TYPE::ERK_SERVICE_HOME)
	{
		ConsistentHashChecker::instance().sync_balance_services(svrs);
	}
}

void HomeServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< home service node:%lld online to eureka >>>>>>>>>>>>", sid);

	ConsistentHashChecker::instance().init_myid(sid);

	this->is_ready_ = true;
}

void HomeServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "home service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}
