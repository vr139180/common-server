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
	if (type == NETSERVICE_TYPE::ERK_SERVICE_RES)
	{
		res_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void HomeServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "home service[%lld] registed to eureka, success............", sid);
	this->is_ready_ = true;
}

void HomeServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "home service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}

void HomeServiceApp::on_mth_gatebindhome_confirm(BasicProtocol* message, bool& autorelease)
{
	autorelease = false;

	PRO::Svr_GateBindHome_confirm *cf = dynamic_cast<PRO::Svr_GateBindHome_confirm*>(message);
	cf->set_homeiid(EurekaClusterClient::instance().get_myiid());
	cf->set_hometoken(EurekaClusterClient::instance().get_token());

	EurekaClusterClient::instance().send_mth_protocol( message);
}

void HomeServiceApp::on_mth_userproxyslot_req(BasicProtocol* message, bool& autorelease)
{
	Res_ProxyUserSlot_req* req = dynamic_cast<Res_ProxyUserSlot_req*>(message);

	logDebug(out_runtime, "recv res service userproxy slot request userid:%lld token:%s", req->user_iid(), req->proxytoken().c_str());

	EurekaClusterClient& ec = EurekaClusterClient::instance();
	if (req->homeiid() != ec.get_myiid())
	{
		Res_ProxyUserSlot_ack* ack = new Res_ProxyUserSlot_ack();
		ack->set_result(1);
		ack->set_proxytoken(req->proxytoken().c_str());
		ack->set_homeiid(req->homeiid());
		ack->set_gateiid(req->gateiid());
		ack->set_hgtype(req->hgtype());
		ack->set_lsno(req->lsno());
		ack->set_sequence(req->sequence());
		ack->set_user_iid(req->user_iid());

		send_protocol_to_res(ack);
		return;
	}

	logDebug(out_runtime, "recv res service userproxy slot request userid:%lld token:%s and route to gate service", 
		req->user_iid(), req->proxytoken().c_str());

	autorelease = false;
	send_protocol_to_gate(message);
}
