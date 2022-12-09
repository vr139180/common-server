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

#include <cmsLib/util/Random.h>

#include "ServiceRouterApp.h"

//�Զ���custom channel��channel����serviceid+sequence���ɣ���serviceid��Ψһ�Ա�֤���к�Ψһ
//20bit servicei 43bit
#define CUSTOMCHANNEL_MASK_ZERO_H	0b0000000000000000000001111111111111111111111111111111111111111111
#define CUSTOMCHANNEL_MASK_ZERO_L	0b1111111111111111111110000000000000000000000000000000000000000000

ChatModule& ChatModule::instance()
{
	static ChatModule s_instance;
	return s_instance;
}

ChatModule::ChatModule() :channel_seed_(0)
{
	iids_ = 0;
}

void ChatModule::init_chatmodule(S_INT_64 myiid)
{
	this->channel_seed_ = myiid;
	this->iids_ = (S_INT_64)CMS_RandMinMax(10, 1000);
}

S_INT_64 ChatModule::new_custom_channelid()
{
	this->iids_++;
	return (((channel_seed_ << 43)&CUSTOMCHANNEL_MASK_ZERO_L) | iids_);
}

void ChatModule::send_to_chat( S_INT_64 channelid, const SProtocolHead& head, BasicProtocol* msg)
{
	NetProtocol* pro = new NetProtocol(head, msg);

	SProtocolHead& wh = pro->write_head();
	wh.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER;
	wh.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_CHAT;

	svrApp.router_to_chat(channelid, pro);
}

void ChatModule::send_to_chat(S_INT_64 channelid, NetProtocol* pro)
{
	SProtocolHead& wh = pro->write_head();
	wh.from_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_SVRROUTER;
	wh.to_type_ = (S_INT_8)NETSERVICE_TYPE::ERK_SERVICE_CHAT;

	svrApp.router_to_chat(channelid, pro);
}
