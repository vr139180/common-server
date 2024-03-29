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

//vnode必须一致
#define VNODE_HOMESERVICE_NUM	800
#define VNODE_CHATSERVICE_NUM	800
#define VNODE_MAILSERVICE_NUM	800
#define VNODE_FRIENDSERVICE_NUM	800
//cluster最大循环次数.超出之后router端drop掉请求
#define VNODE_MAX_CIRCLES		3


//gate单个支持的用户连接数
#define GATE_PLAYER_MAX			10000
#define GATE_PIECE_NUM_MAX		2000
//gate扩展属性
#define GATE_INSTANCE_EXTPARAM	"linktype"
//gate外网ip+port
#define GATE_WIP				"wip"
#define GATE_WPORT				"wport"

//home正常设计容量
#define HOME_LOBBY_THREADNUM	1				//lobby线程数
#define HOME_LOBBY_PLAYERS		1000			//每个lobby的用户数
#define HOME_LOBBUSER_TIMEOUT	20*60*60*1000	//20 hour不活跃清楚数据

//eureka定义 8seconds
#define EUREKA_LOST_TIMEOUT		8*1000
#define EUREKA_LOST_RETRY		8

//state容量设计
#define STATE_SERVICE_THREADNUM		4		//state服务线程数
#define STATE_ONLINEQUEUE_HASHNUM	1		//-USERS:ONLINE:#hash的取模数

//game容量设计
#define GAME_CHANNEL_MAXPLAYER		1000	//gameservice each channel, max players

#endif //__GLOBAL_CONST_H__
