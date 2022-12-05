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

#ifndef __GLOBAL_REDIS_CONST_H__
#define __GLOBAL_REDIS_CONST_H__

#include <string>

#define REDISKEY_GLOBAL_NS_BEGIN namespace rdkey { namespace global {
#define REDISKEY_GLOBAL_NS_END } }

#define USED_REDISKEY_GLOBAL_NS using namespace rdkey::global;

#define REDIS_KEY_DEF( KEY, VAL) static const char* KEY = VAL
#define REDIS_FIELD_DEF( KEY, VAL) static const char* KEY = VAL

REDISKEY_GLOBAL_NS_BEGIN

//----------------------------全局自增id-----------------------------------
REDIS_KEY_DEF( GLOBAL_IDGEN_HKEY, "-GLOBAL:IDGEN-");
REDIS_FIELD_DEF( GLOBAL_IDGEN_DBID, "_db_id:%s");

//----------------------------分布式任务 前缀-------------------------------
REDIS_KEY_DEF(GLOBAL_DTASK_PREFIX, "-DTASK:#%s");

//eureka 状态维护任务
REDIS_KEY_DEF(GLOBAL_DTASK_EUREKAMAINTNCE, "eureka-maintnce");

//------------------------sEureka cluster相关------------------------------
//存放eureka master节点信息, hash
REDIS_KEY_DEF(EUREKA_MASTER_NODE, "_EUREKA:MASTER");
REDIS_FIELD_DEF(FIELD_MASTER_NODE_IID, "iid");
REDIS_FIELD_DEF(FIELD_MASTER_NODE_SVR, "svrinfo");

//master节点失效时间 4 seconds
#define EUREKA_MASTER_NODE_TIMEOUT	4*1000

//gateinfo , key 保存GateNodeInfo的json格式信息
REDIS_KEY_DEF(GATE_NODE, "-GATE:NODE%lld");
//不同的gate类型的 zset结构
//tcp 连接
REDIS_KEY_DEF(GATE_TCP_LIST, "-GATE:TCP-LIST");
//websocket连接
REDIS_KEY_DEF(GATE_WS_LIST, "-GATE:WS-LIST");
//15秒失效
#define GATENODE_TIMEOUT	15*1000

REDISKEY_GLOBAL_NS_END

#endif //__GLOBAL_REDIS_CONST_H__