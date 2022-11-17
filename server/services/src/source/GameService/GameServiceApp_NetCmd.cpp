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

#include "GameServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void GameServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE type, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
	logDebug(out_runtime, "recv new service type:%s node:%d unvalide node:%d from eureka",
		NetServiceType::to_string(type).c_str(), nodes.size(), deliids.size());

	if (type == NETSERVICE_TYPE::ERK_SERVICE_FIGHTROUTER)
	{
		fightrouter_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void GameServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< game service node:%lld online to eureka >>>>>>>>>>>>", sid);

	this->is_ready_ = true;
}

void GameServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "game service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}
