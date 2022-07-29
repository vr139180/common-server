#include "GameServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void GameServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{

}

void GameServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "game service[%lld] registed to eureka, success............", sid);
	this->is_ready_ = true;
}

void GameServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "game service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}
