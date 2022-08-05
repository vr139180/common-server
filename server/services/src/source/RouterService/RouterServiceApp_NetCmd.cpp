#include "RouterServiceApp.h"

#include <cmsLib/net/NetDriverX.h>
#include <cmsLib/base/OSSystem.h>
#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>

#include "chatmodule/ChatModule.h"
#include "mailmodule/MailModule.h"
#include "friends/FriendModule.h"

USE_PROTOCOL_NAMESPACE

void RouterServiceApp::on_mth_servicebindservice_req(BasicProtocol* pro, bool& autorelease, void* session)
{
	Svr_ServiceBindService_req* req = dynamic_cast<Svr_ServiceBindService_req*>(pro);
	RouterSession* psession = reinterpret_cast<RouterSession*>(session);

	Svr_ServiceBindService_ack* ack = new Svr_ServiceBindService_ack();
	ack->set_result(1);
	ack->set_svr_type(NETSERVICE_TYPE::ERK_SERVICE_ROUTER);
	ack->set_toiid(req->toiid());
	ack->set_totoken(req->totoken());

	//service没有变化
	if (req->toiid() != EurekaClusterClient::instance().get_myiid()
		|| req->totoken() != EurekaClusterClient::instance().get_token())
	{
		ack->set_result(1);
		psession->send_protocol(ack);
		return;
	}

	NETSERVICE_TYPE ctype = (NETSERVICE_TYPE)req->svr_type();
	if (ctype >= NETSERVICE_TYPE::ERK_SERVICE_MAX || ctype <= NETSERVICE_TYPE::ERK_SERVICE_NONE)
	{
		ack->set_result(1);
		psession->send_protocol(ack);
		return;
	}

	//来自gate的注册
	if (ctype == NETSERVICE_TYPE::ERK_SERVICE_GATE)
	{
		ThreadLockWrapper guard(get_threadlock());

		session_from_.remove_waitsession_mth(psession);
		GateServiceLinkFrom *pLink = gate_links_from_.ask_free_link();

		pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//设置当前gatelinke
		gate_links_from_.regist_onlinelink(pLink);

		pLink->registinfo_tolog(true);

		ack->set_result(0);
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_CHAT)
	{
		ThreadLockWrapper guard(get_threadlock());

		session_from_.remove_waitsession_mth(psession);
		ChatServiceLinkFrom *pLink = chat_links_from_.ask_free_link();

		pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//设置当前gatelinke
		chat_links_from_.regist_onlinelink(pLink);

		pLink->registinfo_tolog(true);

		ack->set_result(0);
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_MAIL)
	{
		ThreadLockWrapper guard(get_threadlock());

		session_from_.remove_waitsession_mth(psession);
		MailServiceLinkFrom *pLink = mail_links_from_.ask_free_link();

		pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//设置当前gatelinke
		mail_links_from_.regist_onlinelink(pLink);

		pLink->registinfo_tolog(true);

		ack->set_result(0);
	}
	else if (ctype == NETSERVICE_TYPE::ERK_SERVICE_FRIEND)
	{
		ThreadLockWrapper guard(get_threadlock());

		session_from_.remove_waitsession_mth(psession);
		FriendServiceLinkFrom *pLink = friend_links_from_.ask_free_link();

		pLink->set_linkbase_info(req->myiid(), req->mytoken(), req->myexts());

		psession->auth();
		pLink->set_session(psession);
		psession->set_netlinkbase(pLink);

		//设置当前gatelinke
		friend_links_from_.regist_onlinelink(pLink);

		pLink->registinfo_tolog(true);

		ack->set_result(0);
	}
	else // other sevices
	{

	}

	psession->send_protocol(ack);
}

//-------------------------------------------------------eureka cluster---------------------------------------
void RouterServiceApp::mth_notify_servicenode_new(NETSERVICE_TYPE,
	std::list<ServiceNodeInfo*>& nodes, std::list<S_INT_64>& deliids)
{

}

void RouterServiceApp::mth_service_registed(S_INT_64 sid)
{
	logInfo(out_runtime, "<<<<<<<<<<<< router service node:%lld online to eureka >>>>>>>>>>>>", sid);
	
	this->is_ready_ = true;

	//sid作为custom channel id的种子
	ChatModule::instance().init_chatmodule(sid);
	MailModule::instance().init_mailmodule(sid);
	FriendModule::instance().init_friendmodule(sid);
}

void RouterServiceApp::mth_eureka_losted()
{
	this->is_ready_ = false;

	logError(out_runtime, "router service[%lld] lost all connections of eureka, service will shutdown......", EurekaClusterClient::instance().get_myiid());
	this->quit_app();
}