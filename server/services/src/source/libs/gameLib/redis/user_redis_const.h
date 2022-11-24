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

#ifndef __USER_REDIS_CONST_H__
#define __USER_REDIS_CONST_H__

#include <string>

#define REDISKEY_USER_NS_BEGIN namespace rdkey { namespace user {
#define REDISKEY_USER_NS_END } }

#define USED_REDISKEY_USER_NS using namespace rdkey::user;

#define REDIS_KEY_DEF( KEY, VAL) static const char* KEY = VAL
#define REDIS_FIELD_DEF( KEY, VAL) static const char* KEY = VAL

//--------------------------------------------------------------------------------
REDISKEY_USER_NS_BEGIN

//---------------------------登陆相关-------------------------------------
//和账户相关的信息 hash %s=account
REDIS_KEY_DEF(USER_ACCOUNT, "-LOGIN:ACC:#%s");
REDIS_FIELD_DEF(USER_ACCOUNT_F_USERID, "userid");
REDIS_FIELD_DEF(USER_ACCOUNT_F_TOKEN, "token");

//userid账户相关 hash %lld=userid
REDIS_KEY_DEF(USER_USERINFO, "-LOGIN:USR:#%lld");
REDIS_FIELD_DEF(USER_UINFO_F_USERID, "userid");
REDIS_FIELD_DEF(USER_UINFO_F_TOKEN, "token");
REDIS_FIELD_DEF(USER_UINFO_F_DISABLE, "disable");
REDIS_FIELD_DEF(USER_UINFO_F_LOGINTIME, "logintime");
REDIS_FIELD_DEF(USER_UINFO_F_RELOGIN, "relogin");
REDIS_FIELD_DEF(USER_UINFO_F_ROLES, "roles");
//以下和session相关
REDIS_FIELD_DEF(USER_UINFO_F_GIDUID, "giduid");
REDIS_FIELD_DEF(USER_UINFO_F_SLOTTOKEN, "slottoken");
REDIS_FIELD_DEF(USER_UINFO_F_ROLEID, "roleid");
REDIS_FIELD_DEF(USER_UINFO_F_GAMEID, "gameid");

//在线用户信息 set %d=0
//score userid, value timestamp
REDIS_KEY_DEF(USER_ONLINES, "-LOGIN:ONLINE:#%d");

//用户激活定时 10s
#define USER_ACTIVE_TIME_STEP	10*1000
//断线超时 30s
#define USER_LOSTCONN_TIME		30*1000

//---------------------------用户数据相关-----------------------------------
//datasync 使用的用户数据变更set
//memeber roleiid, field timestamp
REDIS_KEY_DEF(DATASYNC_USERS, "-DATASYNC-:users");


//table id generator
REDIS_KEY_DEF(DBID_ROLE_BASEINFO, "role_baseinfo");
REDIS_KEY_DEF(DBID_USER_HOME_STRUCTURE, "user_home_structure");
REDIS_KEY_DEF(DBID_USER_PETS, "user_pets");

REDIS_KEY_DEF(DBID_USER_TASKGROUP, "user_taskgroup");
REDIS_KEY_DEF(DBID_USER_TASKINFO, "user_taskinfo");
//life time 14 days

#define REDIS_USER_LIFETIME	14*24*60*60*1000

//role detail lld=role_iid
REDIS_KEY_DEF(USER_DETAIL, "-USERS:#%lld");
//userbase
REDIS_FIELD_DEF(USER_DETAIL_USERBASE, "user:base");
REDIS_FIELD_DEF(USER_DETAIL_USERHOME, "user:home");
REDIS_FIELD_DEF(USER_DETAIL_BUILDING, "build:items");
REDIS_FIELD_DEF(USER_DETAIL_BUILDING_DEL, "build:dels");
REDIS_FIELD_DEF(USER_DETAIL_PET, "pet:items");
REDIS_FIELD_DEF(USER_DETAIL_PET_DEL, "pet:dels");

REDIS_FIELD_DEF(USER_DETAIL_TASKS, "tsk:tsks");
REDIS_FIELD_DEF(USER_DETAIL_TASKS_END, "tsk:tsks_end");
REDIS_FIELD_DEF(USER_DETAIL_TASKGROUPS, "tsk:groups");
REDIS_FIELD_DEF(USER_DETAIL_TASKGROUPS_END, "tsk:groups_end");

//数据版本控制项
//redis上的版本号
REDIS_FIELD_DEF(USER_DETAIL_VERSION_RD, "ver:rv");
REDIS_FIELD_DEF(USER_DETAIL_VERSION_DB, "ver:db");
//更改来源,记录home service iid
REDIS_FIELD_DEF(USER_DETAIL_VERSION_SOURCE, "ver:src");

REDISKEY_USER_NS_END
//--------------------------------------------------------------------------------

#endif //__USER_REDIS_CONST_H__