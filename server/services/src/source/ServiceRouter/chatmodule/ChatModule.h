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

#ifndef __CHATMODULE_H__
#define __CHATMODULE_H__

#include <boost/atomic.hpp>
#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>

#include "config/RouterConfig.h"

class ChatModule
{
private:
	ChatModule();
public:
	static ChatModule& instance();

	void init_chatmodule(S_INT_64 myiid);

	void process_chat_msg( NetProtocol* pro);

protected:
	void on_customchannelid_req(const SProtocolHead& head);

	void on_userchannel_active(NetProtocol* pro);
	void on_user_say_somthing(NetProtocol* pro);

	void send_to_chat(S_INT_64 channelid, const SProtocolHead& head, BasicProtocol* msg);
	void send_to_chat(S_INT_64 channelid, NetProtocol* pro);

protected:
	//新建新的编号
	S_INT_64 new_custom_channelid();

private:
	//sid作为custom channel id的种子
	S_INT_64				channel_seed_;
	boost::atomic<S_INT_64>	iids_;
};

#endif //__CHATMODULE_H__
