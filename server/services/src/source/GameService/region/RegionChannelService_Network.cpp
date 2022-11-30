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

#include "region/RegionChannelService.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/protobuf/ProtoUtil.h>

#include <gameLib/LogExt.h>
#include <gameLib/commons/GLoc3D.h>

#include "GameServiceApp.h"

USE_PROTOCOL_NAMESPACE

void RegionChannelService::InitNetMessage()
{
	REGISTERMSG(GMS_PROTYPE::GMS_ENTERGAME_REQ, &RegionChannelService::on_gate_enter_game_req, this);
	REGISTERMSG(GMS_PROTYPE::GMS_USERALIVE_NTF, &RegionChannelService::on_gate_userlive_ntf, this);

	REGISTERMSG(USER_PROTYPE::USER_MYSIMPLEINFO_ACK, &RegionChannelService::on_home_mysimpleinfo_ack, this);

	REGISTERMSG(GMS_PROTYPE::GMS_USERSTATE_SYN, &RegionChannelService::on_pl_userstate_sync, this);
}

void RegionChannelService::on_gate_userlive_ntf(NetProtocol* pro, bool& autorelease)
{
	//激活用户
	GamePlayer* puser = channel_users_.get_gameuser_exist(pro->get_useriid(), true);
}

void RegionChannelService::on_gate_enter_game_req(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = channel_users_.get_gameuser(pro->get_useriid());
	if (puser == 0)
		return;

	GLoc3D loc;
	ProtoUtil::get_location_from_msg(pro->msg_, loc);

	//更新头信息,只在entergame时触发
	puser->sync_head(pro->head_, gameid_, loc);
}

void RegionChannelService::on_home_mysimpleinfo_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = channel_users_.get_gameuser_exist(pro->get_useriid());
	if (puser == 0)
		return;


}

void RegionChannelService::on_pl_userstate_sync(NetProtocol* pro, bool& autorelease)
{

}