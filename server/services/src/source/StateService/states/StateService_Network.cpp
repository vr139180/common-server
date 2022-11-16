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

#include "states/StateService.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "StateServiceApp.h"

USE_PROTOCOL_NAMESPACE

void StateService::InitNetMessage()
{
	REGISTERMSG(USER_PROTYPE::USER_LOGIN_REQ, &StateService::on_user_login_req, this);
	REGISTERMSG(USER_PROTYPE::USER_RELOGIN_REQ, &StateService::on_user_relogin_req, this);
	REGISTERMSG(USER_PROTYPE::USER_ACTIVE_NTF, &StateService::on_user_active_ntf, this);
}

void StateService::on_user_login_req(NetProtocol* pro, bool& autorelease)
{
	User_Login_req* req = dynamic_cast<User_Login_req*>(pro->msg_);

	logDebug(out_runtime, "recv user:%s login req...", req->account().c_str());
}

void StateService::on_user_relogin_req(NetProtocol* pro, bool& autorelease)
{
	User_ReLogin_req* req = dynamic_cast<User_ReLogin_req*>(pro->msg_);
}

void StateService::on_user_active_ntf(NetProtocol* pro, bool& autorelease)
{
	User_Active_ntf* ntf = dynamic_cast<User_Active_ntf*>(pro->msg_);
}
