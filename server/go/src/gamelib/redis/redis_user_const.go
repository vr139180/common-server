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

package redis

//datasync 使用的用户数据变更set
const DATASYNC_USERS = "-DATASYNC-:users"
const DATASYNC_USERS_CONFIRM = "-DATASYNC-:users-confirm"

//-----------------------user detail--------------------------
//role detail lld=role_iid
const USER_DETAIL = "-UROLE:#%s"

//userbase
const USER_DETAIL_USERBASE = "user:base"
const USER_DETAIL_USERHOME = "user:home"
const USER_DETAIL_BUILDING = "build:items"
const USER_DETAIL_BUILDING_DEL = "build:dels"
const USER_DETAIL_PET = "pet:items"
const USER_DETAIL_PET_DEL = "pet:dels"

const USER_DETAIL_TASKS = "tsk:tsks"
const USER_DETAIL_TASKS_END = "tsk:tsks_end"
const USER_DETAIL_TASKGROUPS = "tsk:groups"
const USER_DETAIL_TASKGROUPS_END = "tsk:groups_end"

//数据版本控制项
//redis上的版本号
const USER_DETAIL_VERSION_RD = "ver:rv"
const USER_DETAIL_VERSION_DB = "ver:db"

//更改来源,记录home service iid
const USER_DETAIL_VERSION_SOURCE = "ver:src"

//------------------------------------------------------------
