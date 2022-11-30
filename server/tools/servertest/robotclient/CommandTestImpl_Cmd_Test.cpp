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

#include "StdAfx.h"

#include "CommandTestImpl.h"
#include <cmsLib/GlobalSettings.h>
#include <cmsLib/util/ShareUtil.h>

USE_PROTOCOL_NAMESPACE;


void CommandTestImpl::game_userstate_sync()
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Game_UserState_sync* sync = new Game_UserState_sync();
	sync->set_user_iid(user_iid_);

	if (!send_to_gts(sync))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_game_userstate_sync(BasicProtocol* pro, CString* pRetMsg)
{
	Game_UserState_sync* ack = dynamic_cast<Game_UserState_sync*>(pro);
}

void CommandTestImpl::on_game_users_sync(BasicProtocol* pro, CString* pRetMsg)
{
	Game_UsersVisiable_ntf* ack = dynamic_cast<Game_UsersVisiable_ntf*>(pro);
}
