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

#include "dbs/cmd/BaseDBCmd.h"

#include "lobby/LobbyService.h"

BaseDBCmd::BaseDBCmd(S_INT_64 uid, S_INT_64 token, LobbyService* p):user_iid_(uid)
,protoken_( token)
,lobby_( p)
{
}

void BaseDBCmd::dispath_again()
{
	lobby_->regist_syscmd(this);
}