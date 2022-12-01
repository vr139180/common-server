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

#include "region/RegionCellNode.h"
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
	//�����û�
	channel_users_.get_gameuser_exist(pro->get_useriid(), true);
}

void RegionChannelService::on_gate_enter_game_req(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = channel_users_.get_gameuser(pro->get_useriid());
	if (puser == 0)
		return;

	GLoc3D loc;
	ProtoUtil::get_location_from_msg(pro->msg_, loc);

	//����ͷ��Ϣ,ֻ��entergameʱ����
	puser->enter_game(pro->head_, gameid_, loc);
	//ͬ������ʽ
	region_map_->user_enter_region(puser);

	Game_EnterGame_ack *ack = new Game_EnterGame_ack();
	ack->set_result(0);
	ack->set_game_iid(gameid_);
	ProtoUtil::set_location_to_msg(ack, puser->get_location());

	puser->send_to_gate(ack);
}

void RegionChannelService::on_home_mysimpleinfo_ack(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = channel_users_.get_gameuser_exist(pro->get_useriid());
	if (puser == 0)
		return;

	if (!puser->on_myinfo_get(pro->msg_))
		return;

	if (puser->get_region_owner() == 0)
		return;

	//�㲥�����Ϣ����
	Game_UserInfo_sync* sync = new Game_UserInfo_sync();
	std::unique_ptr<Game_UserInfo_sync> xptr(sync);
	PRO::GameUserInfo* pui = sync->mutable_info();
	puser->copy_user_info(pui);

	puser->get_region_owner()->broadcast(sync);
}

void RegionChannelService::on_pl_userstate_sync(NetProtocol* pro, bool& autorelease)
{
	GamePlayer* puser = channel_users_.get_gameuser_exist(pro->get_useriid());
	if (puser == 0)
		return;

	GLoc3D loc;
	ProtoUtil::get_location_from_msg(pro->msg_, loc);
	puser->set_location(loc);
	
	//move
	region_map_->user_move_region(puser);

	region_map_->user_view_broadcast( puser, pro->msg_);
}