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
	if (type == NETSERVICE_TYPE::ERK_SERVICE_ROUTER)
	{
		//logDebug(out_net, "eureka subscrite service change, type:%d, new:%d, del:%d", type, nodes.size(), deliids.size());
		router_link_mth_.sync_eureka_services(nodes, deliids);
	}
}

void GateServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_sys, "gate service[%lld] registed to eureka, success............", sid);

	//第一次同步成功之后，发起home绑定请求
	if (bind_home_step == GateBindHomeStep::GateBindHome_Waiting)
	{
		bind_home_step = GateBindHomeStep::GateBindHome_CanAskBind;
	}
}

void GateServiceApp::mth_eureka_losted()
{
	logError(out_sys, "gate service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}

void GateServiceApp::on_mth_gatebindhome_ack(bool succ, ServiceNodeInfo* pnode, S_INT_64 gateid, S_INT_64 bindtoken)
{
	if (succ)
	{
		std::unique_ptr<ServiceNodeInfo> ptr(pnode);
		if (gateid != EurekaClusterClient::instance().get_myiid() || bind_home_step != GateBindHomeStep::GateBindHome_AskBind)
			return;

		//准备确认
		bind_home_step = GateBindHomeStep::GateBindHome_Confirm;

		home_linkto_->reset(ptr.release(), bindtoken);
		home_linkto_->connect();
	}
	else
	{
		if (gateid != EurekaClusterClient::instance().get_myiid())
			return;
		//失败后，通过定时器重新发起请求.TODO:超时未响应，重新发起请求
		bind_home_step = GateBindHomeStep::GateBindHome_CanAskBind;
	}
}

void GateServiceApp::on_disconnected_with_player(GateSession* psession)
{
	{
		ThreadLockWrapper guard(lock_);

		psession->reset();
		session_from_.return_freesession_mth(psession);
	}

	//同步freeslot
	GamePlayerCtrl::instance().on_mth_gateslot_sync( 0);
}

void GateServiceApp::on_player_login(GateSession* psession)
{
	ThreadLockWrapper guard(lock_);

	psession->auth();
	session_from_.remove_waitsession_mth(psession);
}