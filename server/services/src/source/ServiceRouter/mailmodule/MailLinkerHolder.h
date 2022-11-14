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

#ifndef __MAILLINKERHOLDER_H__
#define __MAILLINKERHOLDER_H__

#include <gameLib/commons/LinkFromHolder.h>
#include "network/MailServiceLinkFrom.h"

class MailLinkerHolder : public LinkFromHolder<MailServiceLinkFrom>
{
	typedef LinkFromHolder<MailServiceLinkFrom> base;
public:
	MailLinkerHolder();

	virtual void uninit_holder();

	virtual void return_freelink(MailServiceLinkFrom* link);
	virtual MailServiceLinkFrom* regist_onlinelink(MailServiceLinkFrom* link);

	void send_mth_protocol(int mailhash, BasicProtocol* pro);
	void send_mth_protocol_circle(BasicProtocol* pro);

protected:
	boost::unordered_map<int, MailServiceLinkFrom*>	hash_service_;
	std::vector<MailServiceLinkFrom*>	mails_vector_;
	int cur_mail_service_;
};

#endif //__CHATLINKERHOLDER_H__
