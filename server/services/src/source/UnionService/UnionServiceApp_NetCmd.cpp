#include "UnionServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void UnionServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{

}

void UnionServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "union service[%lld] registed to eureka, success............", sid);
	this->is_ready_ = true;
}

void UnionServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "union service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}