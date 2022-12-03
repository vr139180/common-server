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

void CommandTestImpl::game_user_entergame()
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Game_EnterGame_req* sync = new Game_EnterGame_req();

	if (!send_to_gts(sync))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_game_enter_ack(BasicProtocol* pro, CString* pRetMsg)
{
	Game_EnterGame_ack* ack = dynamic_cast<Game_EnterGame_ack*>(pro);
	CString str1;
	str1.Format("用户进入游戏场景 result:%d\r\n", ack->result());
	*pRetMsg += str1;
}

void CommandTestImpl::on_game_userinfo_sync(BasicProtocol* pro, CString* pRetMsg)
{
	Game_UserInfo_sync* sync = dynamic_cast<Game_UserInfo_sync*>(pro);
	CString str1;
	str1.Format("用户信息同步\r\n");
	*pRetMsg += str1;
}

void CommandTestImpl::on_game_usersinout_sync(BasicProtocol* pro, CString* pRetMsg)
{
	Game_UsersVisiable_ntf* ack = dynamic_cast<Game_UsersVisiable_ntf*>(pro);
	CString str1;
	str1.Format("用户in out 通知 full:%d in:%d out:%d\r\n", ack->full(), ack->online_users_size(), ack->offline_users_size());
	*pRetMsg += str1;
}

void CommandTestImpl::game_userstate_sync(int x, int z)
{
	ret_desc_ = "";
	if (!islogon())
	{
		ret_desc_ = "用户未登陆\r\n";
		return;
	}

	Game_UserState_sync* sync = new Game_UserState_sync();
	sync->set_user_iid(user_iid_);
	PRO::Location3D* pos = sync->mutable_role_pos();
	pos->set_x((float)x);
	pos->set_z((float)z);
	pos->set_y(0.0);

	if (!send_to_gts(sync))
	{
		ret_desc_ = "发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::on_game_userstate_sync(BasicProtocol* pro, CString* pRetMsg)
{
	Game_UserState_sync* ack = dynamic_cast<Game_UserState_sync*>(pro);
	CString str1;
	str1.Format("用户状态转发 uid:%lld\r\n", ack->user_iid());
	*pRetMsg += str1;
}
