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

#include "mailmodule/MailModule.h"

#include <gameLib/protobuf/Proto_all.h>
#include <gameLib/LogExt.h>

#include "ServiceRouterApp.h"

USE_PROTOCOL_NAMESPACE

void MailModule::process_mail_msg(NetProtocol* pro)
{
	S_UINT_16 msgid = pro->get_msg();
	std::unique_ptr<NetProtocol> p_msg(pro);

	if (msgid == MAIL_PROTYPE::MAIL_SYSTEMMAIL_REQ)
	{
		svrApp.send_protocal_to_mail_circle(p_msg.release());
	}
	else
	{
		svrApp.send_protocal_to_mail(p_msg.release());
	}
}
