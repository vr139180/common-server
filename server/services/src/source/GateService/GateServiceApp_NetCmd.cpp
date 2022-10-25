#include "GateServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/global_const.h>

#include <gameLib/protobuf/Proto_all.h>
#include <player/GamePlayerCtrl.h>

USE_PROTOCOL_NAMESPACE

void GateServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE type, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
	if (type == NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER)
	{
		//logDebug(out_runtime, "eureka subscrite service change, type:%d, new:%d, del:%d", type, nodes.size(), deliids.size());
		datarouter_link_mth_.sync_eureka_services(nodes, deliids);
	}
	else if (type == NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER)
	{
		//logDebug(out_runtime, "eureka subscrite service change, type:%d, new:%d, del:%d", type, nodes.size(), deliids.size());
		svrrouter_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void GateServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< gate service node:%lld online to eureka >>>>>>>>>>>>", sid);
}

void GateServiceApp::mth_eureka_losted()
{
	logError(out_runtime, "gate service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}
