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

#include "friends/FriendModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "ServiceRouterApp.h"

USE_PROTOCOL_NAMESPACE

void FriendModule::process_friend_msg( NetProtocol* pro)
{
	std::unique_ptr<NetProtocol> p_msg(pro);
	logDebug(out_runtime, "resend msg:%d to friend service\r\n", pro->get_msg());
	svrApp.send_protocal_to_friend( p_msg.release());
}
