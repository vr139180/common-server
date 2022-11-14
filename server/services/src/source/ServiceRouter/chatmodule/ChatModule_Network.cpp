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

void ChatModule::process_chat_msg(S_UINT_16 proiid, BasicProtocol* pro)
{
	std::unique_ptr<BasicProtocol> p_msg(pro);
	if (proiid == CHAT_PROTYPE::CHAT_CUSTOMCHANNELID_REQ)
	{
		Chat_CustomChannelId_req* req = dynamic_cast<Chat_CustomChannelId_req*>(pro);
		//on_customchannelid_req(req->utoken().giduid(), req->utoken().slottoken());
	}
	else if (proiid == CHAT_PROTYPE::CHAT_USERCHANNELS_ACTIVE)
	{
		on_userchannel_active(pro);
	}
	else if (proiid == CHAT_PROTYPE::CHAT_USERMSG_SAY)
	{
		on_user_say_somthing(p_msg.release());
	}
}

void ChatModule::on_customchannelid_req(S_INT_64 sidgid, S_INT_64 slottoken)
{
	S_INT_64 gateid = 0;

	S_INT_64 cid = this->new_custom_channelid();
	
	Chat_CustomChannelId_ack *ack = new Chat_CustomChannelId_ack();
	//ProtoUtil::set_usertokenx(ack, sidgid, slottoken);
	ack->set_result(0);

	ChatChannelInfo *info = new ChatChannelInfo();
	info->set_type(ChatChannelType::ChatChannelT_Custom);
	info->set_channeldid(cid);
	ack->set_allocated_channel( info);

	svrApp.send_protocal_to_gate(gateid, ack);
}

void ChatModule::on_userchannel_active(BasicProtocol* pro)
{
	//pro dont destroy
	Chat_UserChannels_active* act = dynamic_cast<Chat_UserChannels_active*>(pro);

	//分拆到各个chats
	//const UserToken& ut = act->utoken();

	//user作为一个单独的channel来激活
	S_INT_64 uid = 0;
	//ProtoTokenUtil::parse_usergate2(ut.giduid(), uid);

	boost::unordered_map<int, Chat_UserChannels_active*> hmsg;
	
	int cid = channelid_to_chathash(ChatChannelType::ChatChannelT_Single, uid);
	
	Chat_UserChannels_active* nd = new Chat_UserChannels_active();
	//ProtoUtil::set_usertokenx(nd, ut.giduid(), ut.slottoken());
	ChatChannelInfo* pi = nd->add_channels();
	pi->set_type(ChatChannelType::ChatChannelT_Single);
	pi->set_channeldid(uid);
	hmsg[cid] = nd;

	for (int ii = 0; ii < act->channels_size(); ++ii)
	{
		const ChatChannelInfo& c = act->channels(ii);
		cid = channelid_to_chathash( c.type(), c.channeldid());

		boost::unordered_map<int, Chat_UserChannels_active*>::iterator fiter = hmsg.find(cid);
		if (fiter == hmsg.end())
		{
			nd = new Chat_UserChannels_active();
			//ProtoUtil::set_usertokenx(nd, ut.giduid(), ut.slottoken());

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
	for (boost::unordered_map<int, Chat_UserChannels_active*>::iterator iter = hmsg.begin();
		iter != hmsg.end(); ++iter)
	{
		int cid = iter->first;
		Chat_UserChannels_active* c = iter->second;

		svrApp.send_protocal_to_chat(cid, c);
	}
}

void ChatModule::on_user_say_somthing(BasicProtocol* pro)
{
	//you need to manage pro object destroy or somthing
	Chat_UserMsg_say* say = dynamic_cast<Chat_UserMsg_say*>(pro);
	const ChatChannelInfo& cl = say->channel();
	int chatiid = channelid_to_chathash(cl.type(), cl.channeldid());

	svrApp.send_protocal_to_chat(chatiid, say);
}
