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

#include "chatmodule/ChatModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "ServiceRouterApp.h"

USE_PROTOCOL_NAMESPACE

void ChatModule::process_chat_msg( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	S_UINT_16 msgid = pro->get_msg();
	if (msgid == CHAT_PROTYPE::CHAT_CUSTOMCHANNELID_REQ)
	{
		on_customchannelid_req(pro->head_);
	}
	else if (msgid == CHAT_PROTYPE::CHAT_USERCHANNELS_ACTIVE)
	{
		on_userchannel_active(pro);
	}
	else if (msgid == CHAT_PROTYPE::CHAT_USERMSG_SAY)
	{
		on_user_say_somthing(p_msg.release());
	}
}

void ChatModule::on_customchannelid_req( const SProtocolHead& head)
{
	Chat_CustomChannelId_ack *ack = new Chat_CustomChannelId_ack();
	ack->set_result(0);

	ChatChannelInfo *info = ack->mutable_channel();
	info->set_type(ChatChannelType::ChatChannelT_Custom);
	S_INT_64 cid = this->new_custom_channelid();
	info->set_channeldid(cid);

	NetProtocol* pro = new NetProtocol( head, ack);
	SProtocolHead& wh = pro->write_head();
	wh.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER;
	wh.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_GATE;

	svrApp.router_to_gate(pro);
}

void ChatModule::on_userchannel_active(NetProtocol* pro)
{
	//用来合并激活协议
	boost::unordered_map<S_INT_64, Chat_UserChannels_active*> hmsg;
	
	//user作为一个单独的channel来激活
	S_INT_64 roleid = pro->get_roleiid();
	Chat_UserChannels_active* nd = new Chat_UserChannels_active();
	ChatChannelInfo* pi = nd->add_channels();
	pi->set_type(ChatChannelType::ChatChannelT_Single);
	pi->set_channeldid(roleid);

	S_INT_64 cid = svrApp.get_chat_by_channelid(roleid);
	hmsg[cid] = nd;

	Chat_UserChannels_active* act = dynamic_cast<Chat_UserChannels_active*>(pro->msg_);
	for (int ii = 0; ii < act->channels_size(); ++ii)
	{
		const ChatChannelInfo& c = act->channels(ii);
		S_INT_64 cid = svrApp.get_chat_by_channelid(c.channeldid());

		boost::unordered_map<S_INT_64, Chat_UserChannels_active*>::iterator fiter = hmsg.find(cid);
		if (fiter == hmsg.end())
		{
			nd = new Chat_UserChannels_active();

			pi = nd->add_channels();
			pi->set_type( c.type());
			pi->set_channeldid( c.channeldid());

			hmsg[cid] = nd;
		}
		else
		{
			nd = fiter->second;
			pi = nd->add_channels();
			pi->set_type(c.type());
			pi->set_channeldid(c.channeldid());
		}
	}

	//router to chat
	for (boost::unordered_map<S_INT_64, Chat_UserChannels_active*>::iterator iter = hmsg.begin(); iter != hmsg.end(); ++iter)
	{
		S_INT_64 cid = iter->first;
		Chat_UserChannels_active* c = iter->second;
		send_to_chat(cid, pro->head_, c);
	}
	hmsg.clear();
}

void ChatModule::on_user_say_somthing(NetProtocol* pro)
{
	//you need to manage pro object destroy or somthing
	Chat_UserMsg_say* say = dynamic_cast<Chat_UserMsg_say*>(pro->msg_);
	const ChatChannelInfo& cl = say->channel();

	send_to_chat(cl.channeldid(), pro);
}
