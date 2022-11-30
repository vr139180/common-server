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
	GamePlayers_Ready,
}GamePlayerState;

class GamePlayer
{
public:
	GamePlayer();
	virtual ~GamePlayer() {}

public:
	void reset();
	
	//更新头信息,只在entergame时触发
	void sync_head(const SProtocolHead& head, S_INT_64 gameid, const GLoc3D& loc);

protected:
	void copy_location(const GLoc3D& loc, PRO::Location3D* pos);

private:

	//多地图融合时确定是主节点还是副节点
	bool			b_master_node_;
	GamePlayerState player_state_;

private:
	SProtocolHead		s_head_;
	PRO::GameUserInfo	user_simple_info_;
};

#endif //__GAMEPLAYER_H__
