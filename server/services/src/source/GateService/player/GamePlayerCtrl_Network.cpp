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

#include "player/GamePlayerCtrl.h"

#include <gameLib/LogExt.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/eureka/EurekaClusterClient.h>

#include "GateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void GamePlayerCtrl::chat_globalmsg_notify(NetProtocol* pro)
{
	for (int ii = 0; ii < channel_nums_; ++ii)
	{
		NetProtocol* msg = 0;
		if (ii < (channel_nums_ - 1))
		{
			msg = pro->clone();
		}
		else
			msg = pro;

		NETCMD_FUN_MAP fun = boost::bind(&PlayerChannel::NetProcessMessage, &(all_channels_[ii]),
			boost::placeholders::_1, boost::placeholders::_2);

		NetCommand *pcmd = new NetCommand(msg, fun);
		all_channels_[ii].regist_netcmd(pcmd);
	}
}
