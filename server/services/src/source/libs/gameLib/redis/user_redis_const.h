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

#define REDIS_KEYDEFINEDX( KEY, VAL) static const char* KEY = VAL

//--------------------------------------------------------------------------------
REDISKEY_USER_NS_BEGIN

//datasync 使用的用户数据变更set
//memeber roleiid, field timestamp
REDIS_KEYDEFINEDX(DATASYNC_USERS, "-DATASYNC-:users");


//table id generator
REDIS_KEYDEFINEDX(DBID_ROLE_BASEINFO, "role_baseinfo");
REDIS_KEYDEFINEDX(DBID_USER_HOME_STRUCTURE, "user_home_structure");
REDIS_KEYDEFINEDX(DBID_USER_PETS, "user_pets");

REDIS_KEYDEFINEDX(DBID_USER_TASKGROUP, "user_taskgroup");
REDIS_KEYDEFINEDX(DBID_USER_TASKINFO, "user_taskinfo");

//life time 14 days
#define REDIS_USER_LIFETIME	14*24*60*60*1000

//lld=user_iid
REDIS_KEYDEFINEDX(USER_ROLES, "-USERS-ROLES:%lld");
//role detail lld=role_iid
REDIS_KEYDEFINEDX(USER_DETAIL, "-USERS:%lld");
//userbase
REDIS_KEYDEFINEDX(USER_DETAIL_USERBASE, "user:base");
REDIS_KEYDEFINEDX(USER_DETAIL_USERHOME, "user:home");
REDIS_KEYDEFINEDX(USER_DETAIL_BUILDING, "build:items");
REDIS_KEYDEFINEDX(USER_DETAIL_BUILDING_DEL, "build:dels");
REDIS_KEYDEFINEDX(USER_DETAIL_PET, "pet:items");
REDIS_KEYDEFINEDX(USER_DETAIL_PET_DEL, "pet:dels");

REDIS_KEYDEFINEDX(USER_DETAIL_TASKS, "tsk:tsks");
REDIS_KEYDEFINEDX(USER_DETAIL_TASKS_END, "tsk:tsks_end");
REDIS_KEYDEFINEDX(USER_DETAIL_TASKGROUPS, "tsk:groups");
REDIS_KEYDEFINEDX(USER_DETAIL_TASKGROUPS_END, "tsk:groups_end");

//数据版本控制项
//redis上的版本号
REDIS_KEYDEFINEDX(USER_DETAIL_VERSION_RD, "ver:rv");
REDIS_KEYDEFINEDX(USER_DETAIL_VERSION_DB, "ver:db");
//更改来源,记录home service iid
REDIS_KEYDEFINEDX(USER_DETAIL_VERSION_SOURCE, "ver:src");

REDISKEY_USER_NS_END
//--------------------------------------------------------------------------------

#endif //__USER_REDIS_CONST_H__