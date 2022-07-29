#include "player/PlayerChannel.h"

#include <gameLib/LogExt.h>
#include <gameLib/eureka/EurekaClusterClient.h>

#include <gameLib/protobuf/Proto_all.h>

#include "player/GamePlayerCtrl.h"
#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void PlayerChannel::InitNetMessage()
{
	REGISTERMSG(USER_PROTYPE::USER_LOGOUT_NTF, &PlayerChannel::on_pc_userlogout_ntf, this);
	REGISTERMSG(USER_PROTYPE::USER_ROLESELECT_ACK, &PlayerChannel::on_pc_roleselect_ack, this);

	REGISTERMSG(CHAT_PROTYPE::CHAT_GLOBALMSG_NTF, &PlayerChannel::on_pc_broadcast_chat_globalmsg, this);
}

void PlayerChannel::on_cth_freeslot_req(BasicProtocol* msg, bool& autorelease, int slot)
{
	Res_ProxyUserSlot_req* req = dynamic_cast<Res_ProxyUserSlot_req*>(msg);

	Res_ProxyUserSlot_ack *ack = new Res_ProxyUserSlot_ack();
	ack->set_proxytoken(req->proxytoken().c_str());
	ack->set_homeiid(req->homeiid());
	ack->set_gateiid(req->gateiid());
	ack->set_hgtype(req->hgtype());
	ack->set_lsno(req->lsno());
	ack->set_sequence(req->sequence());
	ack->set_user_iid(req->user_iid());

	S_INT_64 token = OSSystem::mOS->GetTimestamp();

	GamePlayer* pPlayer = this->ask_free_slot(slot, token, req->user_iid());

	ack->set_result(0);
	ack->set_slot(pPlayer->get_userslot());
	ack->set_proxyip(EurekaClusterClient::instance().get_myip());
	ack->set_proxyport(EurekaClusterClient::instance().get_myport());
	ack->set_slottoken(pPlayer->get_slottoken());

	svrApp.send_to_homeservice(ack);

	logDebug(out_runtime, "recv home service userproxy slot request userid:%lld slot:%d utoken:%lld to home service", 
		req->user_iid(), pPlayer->get_userslot(), pPlayer->get_slottoken());
}

void PlayerChannel::on_disconnected_with_player(GamePlayer* plink)
{
	force_pc_close_player(plink);
}

void PlayerChannel::on_cth_userproxylogin_req(BasicProtocol* message, bool& autorelease, void* p)
{
	GateSession *psession = (GateSession*)p;

	User_ProxyLogin_ack* ack = new User_ProxyLogin_ack();
	ack->set_result(1);

	User_ProxyLogin_req* req = dynamic_cast<User_ProxyLogin_req*>(message);

	GamePlayer* pLink = get_player_byslot( req->slot());

	//token check
	if (pLink->is_samesession(req->proxytoken()) == false || pLink->get_iid() != req->user_iid())
	{
		psession->send_protocol(ack);
		return;
	}

	//auth check
	if (parent_->auth_wait_slot(pLink->get_userslot()) == false)
	{
		psession->send_protocol(ack);
		return;
	}

	online_users_[pLink->get_iid()] = pLink;
	online_user_nums_ = online_users_.size();

	pLink->set_session(psession);
	pLink->registinfo_tolog(true);

	psession->set_netlinkbase(pLink);

	svrApp.on_player_login( psession);

	ack->set_result(0);
	psession->send_protocol(ack);

	//notify home service init user
	GHS_UserInit_req *nreq = new GHS_UserInit_req();
	pLink->set_usertoken(nreq);

	nreq->set_gateiid(EurekaClusterClient::instance().get_myiid());

	svrApp.send_to_homeservice(nreq);

	logDebug(out_runtime, "user:%lld login success", req->user_iid());
}

void PlayerChannel::on_pc_userlogout_ntf(BasicProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	GHS_UserLogout_ntf* ntf = new GHS_UserLogout_ntf();
	puser->set_usertoken(ntf);

	svrApp.send_to_homeservice(ntf);

	force_pc_close_player(puser);
}

void PlayerChannel::on_pc_roleselect_ack(BasicProtocol* pro, bool& autorelease)
{
	GamePlayer *puser = get_player_frommsg(pro);
	if (puser == 0) return;

	User_RoleSelect_ack *ack = dynamic_cast<User_RoleSelect_ack*>(pro);
	if (ack->result() == 0)
	{
		puser->role_selected_done(ack->role_iid(), EurekaClusterClient::instance().get_myiid());

		//ask role detail
		GHS_RoleDetail_ask *ask = new GHS_RoleDetail_ask();
		puser->set_usertoken(ask);

		svrApp.send_to_homeservice(ask);
	}

	autorelease = false;
	puser->send_protocol(pro);
}

void PlayerChannel::on_pc_broadcast_chat_globalmsg(BasicProtocol* pro, bool& autorelease)
{
	Chat_GlobalMsg_ntf *ntf = dynamic_cast<Chat_GlobalMsg_ntf*>(pro);
	for (ONLINE_PLAYER_MAP::iterator iter = online_users_.begin(); iter != online_users_.end(); ++iter)
	{
		GamePlayer *p = iter->second;
		
		Chat_GlobalMsg_ntf* msg = new Chat_GlobalMsg_ntf();
		msg->CopyFrom(*ntf);
		p->send_protocol(pro);
	}
}
