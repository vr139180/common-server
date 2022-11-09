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

#ifndef __GLOBAL_CONST_H__
#define __GLOBAL_CONST_H__

//gate单个支持的用户连接数
#define GATE_PLAYER_MAX			10000
#define GATE_PIECE_NUM_MAX		2000

//home正常设计容量
#define HOME_LOBBY_THREADNUM	4		//lobby线程数
#define HOME_LOBBY_PLAYERS		1000	//每个lobby的用户数

#endif //__GLOBAL_CONST_H__
