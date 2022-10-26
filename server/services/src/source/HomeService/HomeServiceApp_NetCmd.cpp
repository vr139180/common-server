#include "HomeServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

USE_PROTOCOL_NAMESPACE

void HomeServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE type, 
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{
	//sync res service
	if (type == NETSERVICE_TYPE::ERK_SERVICE_DATAROUTER)
	{
		datarouter_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void HomeServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< home service node:%lld online to eureka >>>>>>>>>>>>", sid);

	this->is_ready_ = true;
}

void HomeServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "home service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}
