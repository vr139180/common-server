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

#ifndef __GAMEPLAYER_H__
#define __GAMEPLAYER_H__

#include <cmsLib/core_type.h>
#include <cmsLib/prolib/BasicProtocol.h>
#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/commons/GLoc3D.h>

//用户状态
typedef enum tagGamePlayerState {
	GamePlayerS_Free = 0,
	GamePlayerS_EnterWaitDetail,	//entergame，等待获取详细信息
	GamePlayerS_Ready,
}GamePlayerState;

class RegionCellNode;

class GamePlayer
{
public:
	GamePlayer();
	virtual ~GamePlayer() {}

public:
	void reset();

	void send_to_gate(BasicProtocol* msg);
	void send_to_home(BasicProtocol* msg);
	
	//更新头信息,只在entergame时触发
	bool enter_game(const SProtocolHead& head, S_INT_64 gameid, const GLoc3D& loc);
	bool on_myinfo_get(BasicProtocol* msg);

	//保存位置
	void net_save_gameloc();

public:
	S_INT_64 get_useriid() const { return user_iid_; }
	S_INT_64 get_roleiid() const { return role_iid_; }
	const char* get_nickname() const { return nickname_.c_str(); }
	GLoc3D& get_location() { return location_; }
	void set_location(const GLoc3D& loc);

	RegionCellNode* get_region_owner() { return owner_cellnode_; }
	void unbind_region_owner() { owner_cellnode_ = 0; }
	void bind_region_owner(RegionCellNode*n) { owner_cellnode_ = n; }

	bool is_free() const { return player_state_ == GamePlayerState::GamePlayerS_Free;}
	bool is_ready() const { return player_state_ == GamePlayerState::GamePlayerS_Ready; }

	void copy_user_info(PRO::GameUserInfo* pui);

private:

	//多地图融合时确定是主节点还是副节点
	bool			b_master_node_;
	GamePlayerState player_state_;

	//基本信息
	S_INT_64	user_iid_;
	S_INT_64	role_iid_;
	std::string	nickname_;
	//location
	GLoc3D		location_;
	bool		b_loc_changed_;

	//所属的node
	RegionCellNode*		owner_cellnode_;

private:
	SProtocolHead		s_head_;
};

#endif //__GAMEPLAYER_H__
