#include "player/GamePlayerCtrl.h"

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/eureka/EurekaClusterClient.h>

#include "network/HomeServiceLinkTo.h"

#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void GamePlayerCtrl::on_res_proxyuserslot_req(BasicProtocol* message, bool& autorelease, void* p)
{
	HomeServiceLinkTo *plink = (HomeServiceLinkTo*)p;

	Res_ProxyUserSlot_req* req = dynamic_cast<Res_ProxyUserSlot_req*>(message);

	logDebug(out_runtime, "recv home service userproxy slot request token:%s", req->proxytoken().c_str());

	if (req->gateiid() != EurekaClusterClient::instance().get_myiid())
	{
		Res_ProxyUserSlot_ack *ack = new Res_ProxyUserSlot_ack();
		ack->set_proxytoken(req->proxytoken().c_str());
		ack->set_homeiid(req->homeiid());
		ack->set_gateiid(req->gateiid());
		ack->set_hgtype(req->hgtype());
		ack->set_lsno(req->lsno());
		ack->set_sequence(req->sequence());
		ack->set_user_iid(req->user_iid());

		//gateid不正确
		ack->set_result(2);

		plink->send_protocol(ack);
		return;
	}

	int slot = GamePlayerCtrl::instance().ask_free_slot();
	if (slot == -1)
	{
		Res_ProxyUserSlot_ack *ack = new Res_ProxyUserSlot_ack();
		ack->set_proxytoken(req->proxytoken().c_str());
		ack->set_homeiid(req->homeiid());
		ack->set_gateiid(req->gateiid());
		ack->set_hgtype(req->hgtype());
		ack->set_lsno(req->lsno());
		ack->set_sequence(req->sequence());
		ack->set_user_iid(req->user_iid());

		//没有可用的slot资源
		ack->set_result(3);

		plink->send_protocol(ack);

		//同时发送一个slot数据同步
		on_mth_gateslot_sync(0);

		return;
	}

	PlayerChannel *pchannel = GamePlayerCtrl::instance().get_channel_by_slot(slot);
	NETCMD_FUN_MAP2 fun = boost::bind(&PlayerChannel::on_cth_freeslot_req, pchannel,
		boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

	autorelease = false;
	NetCommand* cmd = new NetCommand(message, fun, slot);
	pchannel->regist_syscmd(cmd);
}

void GamePlayerCtrl::chat_globalmsg_notify(BasicProtocol* msg)
{
	Chat_GlobalMsg_ntf* ntf = dynamic_cast<Chat_GlobalMsg_ntf*>(msg);
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		Chat_GlobalMsg_ntf* msg = 0;
		if (ii < (channel_nums_ - 1))
		{
			msg = new Chat_GlobalMsg_ntf();
			msg->CopyFrom(*ntf);
		}
		else
			msg = ntf;

		NETCMD_FUN_MAP2 fun = boost::bind(&PlayerChannel::NetProcessMessage, &(all_channels_[ii]),
			boost::placeholders::_1, boost::placeholders::_2, boost::placeholders::_3);

		NetCommand *pcmd = new NetCommand(msg, fun, (int)PRO::CHAT_PROTYPE::CHAT_GLOBALMSG_NTF);
		all_channels_[ii].regist_netcmd(pcmd);
	}
}
