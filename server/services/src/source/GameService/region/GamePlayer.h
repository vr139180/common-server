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

//�û�״̬
typedef enum tagGamePlayerState {
	GamePlayerS_Free = 0,
	GamePlayerS_EnterWaitDetail,	//entergame���ȴ���ȡ��ϸ��Ϣ
	GamePlayers_Ready,
}GamePlayerState;

class GamePlayer
{
public:
	GamePlayer();
	virtual ~GamePlayer() {}

public:
	void reset();

private:

	//���ͼ�ں�ʱȷ�������ڵ㻹�Ǹ��ڵ�
	bool	b_master_node_;
	GamePlayerState player_state_;

private:
	S_INT_64	user_iid_;
	S_INT_64	role_iid_;
	//gate��token��Ϣ
	S_INT_64	gate_token_gidsid_;
	S_INT_64	gate_token_slottoken_;
};

#endif //__GAMEPLAYER_H__