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

#ifndef __CMSWORLD_CONST_H__
#define __CMSWORLD_CONST_H__

//region中一个cell的边长 m
#define REGION_CELL_SIZE		30
//人眼能看到的最远距离是3个cell的长度，90m
#define PLAYER_EYESEE_CELL_MAX	3

//步行速度 1.5m/s
#define GAME_PLAYER_WALKSPEED	1.5
//跑步速度 2.5m/s
#define GAME_PLAYER_RUNSPEED	2.5

//-------------------------------------------------------
//地图的制作要求
//长宽必须是30m的整数倍
//地图的原点(0,0)在地图的左下角
//地图切割成的region的长宽必须是30m的整数倍
//地图分隔交汇的地方，如果允许穿越，边缘交汇处扩展3个cell 90m。
//game service的所有请求协议，必须定义optional Location3D role_pos = 1
//-------------------------------------------------------

#endif //__CMSWORLD_CONST_H__
